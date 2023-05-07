//
// Created by Roman Gostilo on 07.05.2023.
//

#ifndef ORDERBOOK_CIRCULAR_BUFFER_H
#define ORDERBOOK_CIRCULAR_BUFFER_H

#include <inttypes.h>
#include <utility>
#include <climits>

struct Node {
    uint64_t price;
    uint64_t volume;
    uint64_t tick_size;
    Node(uint64_t t_p, uint64_t t_v, uint64_t t_t) : price(t_p), volume(t_v), tick_size(t_t) {};
    Node(Node&& other) {
        price = other.price;
        volume = other.volume;
        tick_size = other.tick_size;
    };
    Node(const Node& other) {
        price = other.price;
        volume = other.volume;
        tick_size = other.tick_size;
    };
    Node() {

    };
};

const int DEFAULT_MAX = 10001;

class CircularBuffer {
    Node buf[DEFAULT_MAX];
    size_t cur_size = 0;
    uint64_t minimal = UINT64_MAX;
    uint64_t maximal = 0;
public:
    CircularBuffer() = default;
    size_t erase(const uint64_t key) {
        if (buf[key % DEFAULT_MAX].volume != 0) {
            buf[key % DEFAULT_MAX].volume = 0;
            cur_size--;
        }
    }

    void insert(const std::pair<const uint64_t, Node> &&p) {
        minimal = std::min(minimal, p.first);
        maximal = std::max(maximal, p.first);
        auto key = p.first % DEFAULT_MAX;
        auto n(p.second);
        if (buf[key].volume == 0) {
            cur_size++;
        }
        buf[key].volume = n.volume;
        buf[key].price = n.price;
        buf[key].tick_size = n.tick_size;
    }
    size_t size() {
        return cur_size;
    }

    Node getMax(int index) {
        int id = maximal % DEFAULT_MAX;
        int counter = 0;
        while (1) {
            auto cur = buf[id];
            if (buf[id].volume != 0) {
                counter++;
            }
            if (counter == 19) {
                break;
            }
            id = (id - 1 + DEFAULT_MAX) % DEFAULT_MAX;
        }

        return buf[id];
    }

    Node getMin(int index) {
        int id = minimal % DEFAULT_MAX;
        int counter = 0;
        while (1) {
            if (buf[id].volume != 0) {
                counter++;
            }
            if (counter == 19) {
                break;
            }
            id = (id + 1) % DEFAULT_MAX;
        }

        return buf[id];
    }



    void clear() {
        for (int i = 0; i < DEFAULT_MAX; i++) {
            buf[i].volume = 0;
        }
    }
};

#endif //ORDERBOOK_CIRCULAR_BUFFER_H
