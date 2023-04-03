#include "request_entity.h"
#include <vector>
#include <algorithm>

Entity parse_entity(const std::vector<char>& buffer) {
    auto delimiter = std::find(buffer.begin(), buffer.end(), ':');

    std::string product_name(buffer.begin(), delimiter);
    std::string cost(delimiter + 1, buffer.end());

    return Entity{product_name, cost};
}