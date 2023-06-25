#ifndef SMART_PRICE_TAG_BOT_PARSER_H
#define SMART_PRICE_TAG_BOT_PARSER_H
#include <string>

namespace CallbackParser {

    std::string dumpInlineDumpCallbackButton(std::string dumpStr);

    std::string loadInlineDumpCallbackButton(std::string dumpStr);

//    BotStructure::Tag *loadTagFromSetCommand(std::string tagStr) {
//        std::vector<std::string> tagParts = BotTool::split(tagStr, " ");
//        if (tagParts.size() < 3 + 1) {
//            return nullptr;
//        }
//
////        return BotStructure::createTag()
//        return nullptr;
//    }
}

#endif
