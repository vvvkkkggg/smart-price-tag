#ifndef SERVER_SMART_TAG_ENTITY_H
#define SERVER_SMART_TAG_ENTITY_H

#include <string>
#include "json.hpp"

class Entity {
public:
    Entity(std::string screenId, std::string productName, std::string cost)
            : screenId(std::move(screenId)),
              productName(std::move(productName)),
              cost(std::move(cost)) {}

    std::string screenId;
    std::string productName;
    std::string cost;
};

#endif
