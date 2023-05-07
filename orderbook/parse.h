//
// Created by Roman Gostilo on 03.05.2023.
//

#ifndef ORDERBOOK_PARSE_H
#define ORDERBOOK_PARSE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <optional>

struct Message {
    uint64_t ts;
    int64_t id;
    uint64_t price;
    uint64_t volume;
    uint8_t conf;
};

std::optional<Message> getMessage(std::ifstream &in);

#endif //ORDERBOOK_PARSE_H
