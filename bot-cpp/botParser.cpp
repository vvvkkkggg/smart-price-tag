#include <vector>
#include <string>
#include <boost/algorithm/string/join.hpp>
#include <tgbot/tgbot.h>
#include "json.hpp"
#include "botConsts.h"
#include "botParser.h"
#include "botTools.h"

std::string dumpEmpty(int _) {
    return "";
}

std::string CallbackParser::dumpInlineDumpCallbackButton(std::string dumpStr) {
    return BotConst::DUMP_KEY + dumpStr;
}

std::string *CallbackParser::loadInlineDumpCallbackButton(std::string dumpStr) {
    if (!StringTools::startsWith(dumpStr, BotConst::DUMP_KEY)) {
        return nullptr;
    }

    return new std::string(
            dumpStr.substr(
                    std::strlen(
                            BotConst::DUMP_KEY.c_str()
                    )
            )
    );
}

std::string CallbackParser::dumpInlineSetCallbackButton(std::string setStr) {
    return BotConst::SET_KEY + setStr;
}

std::string CallbackParser::dumpInlineSetTagIdCallbackButton(int id) {
    return "_" + std::to_string(id);
}

struct TagConfirmation *CallbackParser::loadInlineSetCallbackButton(std::string setStr) {
    if (!StringTools::startsWith(setStr, BotConst::SET_KEY)) {
        return nullptr;
    }

    std::vector<std::string> answerParts = BotTool::split(setStr, '_');
    return BotStructure::createTagConfirmation(std::stoi(answerParts[2]), answerParts[1]);
}

std::string CallbackParser::dumpInlineServerDataCallbackButton(std::string setStr) {
    return "server_" + setStr;
}

std::string *CallbackParser::loadInlineServerDataCallbackButton(std::string setStr) {
    if (!StringTools::startsWith(setStr, "server_")) {
        return nullptr;
    }

    return new std::string(BotTool::split(setStr, '_')[1]);
}

Tag *CommandParser::loadTagFromSetCommand(std::string tagStr) {
    std::vector<std::string> tagParts = BotTool::split(tagStr, ' ');

    if (tagParts.size() < 3 + 1) {
        return nullptr;
    }

    try {
        int screeId = std::stoi(tagParts[1]);
        int price = std::stoi(tagParts[2]);
        std::vector<std::string> name(tagParts.begin() + 3, tagParts.end());
        return BotStructure::createTag(screeId, price, boost::algorithm::join(name, " "));
    } catch (std::exception &err) {
        return nullptr;
    }
}

std::string JsonParser::dumpUserToString(User *user) {
    return nlohmann::json{{"host",      user->host},
                          {"secretKey", user->secretKey},
                          {"node",      user->node}}.dump();
}

struct User *JsonParser::loadUserFromStringJson(std::string j) {
    auto _j = nlohmann::json::parse(j);
    return BotStructure::createUser(
            _j["host"],
            _j["secretKey"],
            std::stoi(to_string(_j["node"])
            )
    );
}
