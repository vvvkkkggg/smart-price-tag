#include <string>
#include "botStructures.h"


struct Tag *BotStructure::createTag(int screenId, int price, std::string name) {
    struct Tag *tag = new struct Tag();
    tag->screenId = screenId;
    tag->price = price;
    tag->name = name;
    return tag;
}