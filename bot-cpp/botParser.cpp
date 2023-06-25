#include <tgbot/tgbot.h>
#include "json.hpp"
#include "botConsts.h"
#include "botParser.h"


std::string CallbackParser::dumpInlineDumpCallbackButton(std::string dumpStr) {
    return "dump_" + dumpStr;
}

std::string CallbackParser::loadInlineDumpCallbackButton(std::string dumpStr) {
    if (!StringTools::startsWith(dumpStr, BotConst::DUMP_KEY)) {
        return BotConst::EMPTY_DATA;
    }

    return dumpStr.substr(std::strlen(BotConst::DUMP_KEY.c_str()));
}

//    BotStructure::Tag *loadTagFromSetCommand(std::string tagStr) {
//        std::vector<std::string> tagParts = BotTool::split(tagStr, " ");
//        if (tagParts.size() < 3 + 1) {
//            return nullptr;
//        }
//
////        return BotStructure::createTag()
//        return nullptr;
//    }