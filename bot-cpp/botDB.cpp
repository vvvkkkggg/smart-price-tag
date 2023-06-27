#include <string>
#include <optional>
#include "botDB.h"
#include "botParser.h"
#include "botDialog.h"

struct User *BotDB::getUser(sw::redis::Redis &redis, int userId) {
    std::string userIdStringify = std::to_string(userId);
    auto s = redis.get(userIdStringify);
    return s.has_value() ? JsonParser::loadUserFromStringJson(s.value()) : nullptr;
}

struct User *BotDB::createUser(sw::redis::Redis &redis, int userId, struct User *user) {
    return updateUserNode(redis, userId, user, UserStates::CREATE_USER);
}

struct User *saveChangesById(sw::redis::Redis &redis, int userId, struct User *user) {
    redis.set(std::to_string(userId), JsonParser::dumpUserToString(user));
    return user;
}

struct User *BotDB::updateUserNode(sw::redis::Redis &redis, int userId, struct User *user, int node) {
    user->node = node;
    return saveChangesById(redis, userId, user);
}

struct User *BotDB::updateUserHost(sw::redis::Redis &redis, int userId, struct User *user, std::string host) {
    user->host = host;
    return saveChangesById(redis, userId, user);
}

struct User *BotDB::updateUserSecretKey(sw::redis::Redis &redis, int userId, struct User *user, std::string secretKey) {
    user->secretKey = secretKey;
    return saveChangesById(redis, userId, user);
}
