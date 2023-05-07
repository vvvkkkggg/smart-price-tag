//
// Created by Roman Gostilo on 02.05.2023.
//
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <optional>
#include "parse.h"

enum BlockType {
    SHB = 0x0a0d0d0a,
    IDB = 0x00000001,
    EPB = 0x00000006,
};

int8_t buf[1000];


Message parse_udp(std::ifstream &in, size_t len) {
    in.ignore(8, EOF); // header
    Message m;
    in.read(reinterpret_cast<char *>(&m), len - 8);
    return std::move(m);
}

Message parse_ipv4(std::ifstream &in, size_t len) {
    uint16_t ihl;
    in.read(reinterpret_cast<char *>(&ihl), 1);
    ihl &= 0b1111;
    ihl *= 4;
    in.ignore(ihl - 1, EOF); // skip the rest of the header
    return std::move(parse_udp(in, len - ihl));
}

Message parse_eth(std::ifstream &in, size_t len) {
    in.ignore(6 + 6 + 2, EOF); // skip mac_dest + mac_src + ethtype
    return std::move(parse_ipv4(in, len - 6 - 6 - 2)); // parse len minus all other fields in ethernet frame
};

std::optional<Message> process_block(std::ifstream &in) {
    int32_t type, len;
    in.read(reinterpret_cast<char *>(&type), 4);
    in.read(reinterpret_cast<char *>(&len), 4);

    Message m;

    switch (type) {
        case EPB:
            in.ignore(12, EOF);
            int32_t capt_len, orig_len;
            in.read(reinterpret_cast<char *>(&capt_len), 4);
            in.read(reinterpret_cast<char *>(&orig_len), 4);
            m = parse_eth(in, capt_len);
            in.ignore(len - 8 - 12 - 8 - capt_len, EOF);
            return std::move(m);
        case SHB:
        case IDB:
        default:
            in.ignore(len - 8, EOF);
            return std::nullopt;
    }
}


std::optional<Message> getMessage(std::ifstream &in) {
    while (!in.eof()) {
        std::optional<Message> m = process_block(in);
        if (m.has_value()) {
            return m.value();
        }
    }
    return std::nullopt;
}