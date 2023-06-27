#ifndef SMART_PRICE_TAG_BOT_STRUCTURES_H
#define SMART_PRICE_TAG_BOT_STRUCTURES_H
#include <string>

struct Tag {
    int screenId;
    int price;
    std::string name;
};

struct TagConfirmation {
    int id;
    std::string answer;
};

struct User {
    std::string host;
    std::string secretKey;
    int node;
};

namespace BotStructure {
    struct Tag *createTag(int screenId, int price, std::string name);
    struct TagConfirmation *createTagConfirmation(int Id, std::string answer);
    struct User *createUser(std::string host, std::string secretKey, int node);
}

#endif
