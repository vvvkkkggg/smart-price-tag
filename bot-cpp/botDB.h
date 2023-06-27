#ifndef SMART_PRICE_TAG_BOT_DB_H
#define SMART_PRICE_TAG_BOT_DB_H
#include <sw/redis++/redis++.h>
#include "botStructures.h"

namespace BotDB {
    struct User* getUser(sw::redis::Redis &redis, int userId);
    struct User* createUser(sw::redis::Redis &redis, int userId, struct User *user);
    struct User* updateUserNode(sw::redis::Redis &redis, int userId, struct User *user, int node);
    struct User* updateUserHost(sw::redis::Redis &redis, int userId, struct User *user, std::string host);
    struct User* updateUserSecretKey(sw::redis::Redis &redis, int userId, struct User *user, std::string secretKey);
}

#endif
