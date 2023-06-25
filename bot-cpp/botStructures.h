#ifndef SMART_PRICE_TAG_BOT_STRUCTURES_H
#define SMART_PRICE_TAG_BOT_STRUCTURES_H
#include <string>

struct Tag {
    int screenId;
    int price;
    std::string name;
};

namespace BotStructure {
    struct Tag *createTag(int screenId, int price, std::string name);
}
#endif
