//
// Created by Roman Gostilo on 03.05.2023.
//
#include "parse.h"
#include <errno.h>
#include <string>
#include <map>
#include <cmath>
#include "absl/container/btree_map.h"
#include <boost/pool/pool_alloc.hpp>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <iostream>
#include "circular_buffer.h"
#include <unistd.h>
#include <asm/unistd.h>

#define BTREE

#define PERF

#ifndef CIRC
using alloc = boost::fast_pool_allocator<
        std::pair<const uint64_t, Node>,
        boost::default_user_allocator_malloc_free,
        boost::details::pool::default_mutex,
        32, // boost default
        2000 * sizeof(std::pair<const uint64_t, Node>) // MaxSize, in 88 byte chunks
>;
#endif

#ifdef BTREE
absl::btree_map<uint64_t, Node, std::greater<uint64_t>, alloc> bids;
absl::btree_map<uint64_t, Node, std::less<uint64_t>, alloc> asks;
#endif

#ifdef MAP
std::map <uint64_t, Node, std::greater<uint64_t>, alloc> bids;
std::map <uint64_t, Node, std::less<uint64_t>, alloc> asks;
#endif

#ifdef CIRC
CircularBuffer asks;
CircularBuffer bids;
#endif


enum {
    LIMIT = 10000,
};

enum {
    FINALITY_MASK = 0b1,
    NOT_END = 0b1,
    END = 0b0,
};

enum {
    META_MASK = (uint64_t) 1 << 63,
    META_MESSAGE = (uint64_t) 1 << 63,
    META_T = 0b00000000000000000000000000001111,
};

enum {
    TYPE_MASK = 0b10,
    ASK = 0b10,
    BID = 0b00,
};

void process_snapshot(Message &m, uint64_t &cur_tick_size, std::ifstream &in) {
    // snapshot contains all the state of orderbook
    bids.clear();
    asks.clear();
    std::string cur_inst = reinterpret_cast<char *>(&m.id);  // TODO: useless?
    cur_tick_size = (m.ts & META_T);
    Message snapshot = getMessage(in).value();    // existence of snapshot is guaranteed
    while (1) {
        switch (snapshot.conf & TYPE_MASK) {
            case ASK:
                asks.insert(std::make_pair(snapshot.price, Node(snapshot.price, snapshot.volume, cur_tick_size)));
#ifndef CIRC
                if (asks.size() > LIMIT) {
                    asks.erase(--asks.end());
                }
#endif
                break;
            case BID:
                bids.insert(std::make_pair(snapshot.price, Node(snapshot.price, snapshot.volume, cur_tick_size)));
#ifndef CIRC
                if (bids.size() > LIMIT) {
                    bids.erase(--bids.end());
                }
#endif
                break;
        }
        if ((snapshot.conf & FINALITY_MASK) == END) {
            return;
        }
        snapshot = getMessage(in).value();
    }
}

void process_update(Message m, const uint64_t tick_size, std::ifstream &in) {
    std::string cur_inst = reinterpret_cast<char *>(&m.id);
    while (1) {
        // TODO: probably update does not contain tick_size
        switch (m.conf & TYPE_MASK) {
            case ASK:
                asks.erase(m.price);
                if (m.volume != 0)
                    asks.insert(std::make_pair(m.price, Node(m.price, m.volume, tick_size)));
#ifndef CIRC
                if (asks.size() > LIMIT) {
                    asks.erase(--asks.end());
                }
#endif
                break;
            case BID:
                bids.erase(m.price);
                if (m.volume != 0)
                    bids.insert(std::make_pair(m.price, Node(m.price, m.volume, tick_size)));
#ifndef CIRC
                if (bids.size() > LIMIT) {
                    bids.erase(--bids.end());
                }
#endif
                break;
        }
        if ((m.conf & FINALITY_MASK) == END) {
            return;
        }
        m = getMessage(in).value();
    }
}

long double global_maximum = 0;
//uint64_t global_maximum = 0;

void relax_global_maximum() {

#ifdef CIRC
    auto ask = bids.getMax(20);
    auto bid = asks.getMin(20);

#else
    auto a = asks.begin();
    auto b = bids.begin();
    for (int i = 0; i < 20; i++) {
        ++a;
        ++b;
        if (a == asks.end() || b == bids.end()) {
            return;
        }
    }
    Node ask = a->second;
    Node bid = b->second;
#endif

    long double a_real_price = ((long double) ask.price / pow(10, ask.tick_size));
    long double a_real_volume = ((long double) ask.volume / pow(10, 8));
    long double b_real_price = ((long double) bid.price / pow(10, bid.tick_size));
    long double b_real_volume = ((long double) bid.volume / pow(10, 8));
    long double cur_c = a_real_price * a_real_volume + b_real_price * b_real_volume;

    global_maximum = std::max(global_maximum,
                              cur_c);
};

static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                            int cpu, int group_fd, unsigned long flags) {
    int ret;
    ret = syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
    return ret;
}

int get_event_fd(__u64 config, struct perf_event_attr* pe, __u32 type = PERF_TYPE_SOFTWARE) {
    memset(pe, 0, sizeof(*pe));
    pe->type = type;
    pe->size = sizeof(*pe);
    pe->config = config;
    pe->disabled = 1;
    pe->exclude_kernel = 1;
    pe->exclude_hv = 1;

    int fd = perf_event_open(pe, 0, -1, -1, 0);
    if (fd == -1) {
        fprintf(stderr, "Error opening leader %llx\n", pe->config);
        exit(EXIT_FAILURE);
    }

    ioctl(fd, PERF_EVENT_IOC_RESET, 0);
    ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);
    return fd;
}

long long get_count_stop(int fd) {
    long long count;
    ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
    read(fd, &count, sizeof(count));
    close(fd);
    return count;
}


int main(int argc, char** argv) {

#ifndef PERF
    char cmd[100];
    cmd[0] = '\0';
    strcat(cmd, "lz4 ");
    strcat(cmd, argv[1]);
    system(cmd);
    std::ifstream in(std::string(argv[1]).substr(0, strlen(argv[1]) - 4), std::ifstream::binary);
#endif

#ifdef PERF
    system("lz4 btc.lz4");
    std::ifstream in("btc", std::ifstream::binary);
    struct perf_event_attr pe;
    int fd_loads = get_event_fd(PERF_COUNT_HW_CACHE_REFERENCES, &pe);
    int fd_misses = get_event_fd(PERF_COUNT_HW_CACHE_MISSES, &pe);
    int fd_branch_misses = get_event_fd(PERF_COUNT_HW_BRANCH_MISSES, &pe);
    int fd_instructions = get_event_fd(PERF_COUNT_HW_INSTRUCTIONS, &pe);
    int fd_cycles = get_event_fd(PERF_COUNT_HW_CPU_CYCLES, &pe);
#endif


    uint64_t cur_tick_size;
    while (1) {
        assert(asks.size() < 10002);
        assert(bids.size() < 10002);
        std::optional <Message> m_opt = getMessage(in);
        if (!m_opt.has_value()) {
            break;
        }
        Message message = m_opt.value();
        switch (message.ts & META_MASK) {
            case META_MESSAGE:
                process_snapshot(message, cur_tick_size, in);
                break;
            default:    // UPDATE
                process_update(message, cur_tick_size, in);
                break;
        }
        relax_global_maximum();
    }

    std::cout << global_maximum << std::endl;

#ifdef PERF
    long long cache_ref_count = get_count_stop(fd_loads);
    long long cache_misses_count = get_count_stop(fd_misses);
    printf("REPORT:\n");
    printf("Performed %lld references\n", cache_ref_count);
    printf("Made %lld cache misses\n", cache_misses_count);
    printf("Hit rate is %Lf\n", (long double) (cache_ref_count - cache_misses_count) / (long double) cache_ref_count);
    printf("Made %lld branch misses\n", get_count_stop(fd_branch_misses));
    printf("Executed %lld instructions\n", get_count_stop(fd_instructions));
    printf("Performed %lld cycles\n", get_count_stop(fd_cycles));
#endif
}
