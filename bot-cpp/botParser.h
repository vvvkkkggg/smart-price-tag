#ifndef SMART_PRICE_TAG_BOT_PARSER_H
#define SMART_PRICE_TAG_BOT_PARSER_H
#include <string>
#include "botStructures.h"

namespace CallbackParser {
    std::string dumpEmpty(int _);
    std::string dumpId(int id);

    std::string dumpInlineDumpCallbackButton(std::string dumpStr);
    std::string *loadInlineDumpCallbackButton(std::string dumpStr);

    std::string dumpInlineSetCallbackButton(std::string setStr);
    struct TagConfirmation *loadInlineSetCallbackButton(std::string setStr);
    std::string dumpInlineSetTagIdCallbackButton(int id);
}

namespace CommandParser {
    Tag *loadTagFromSetCommand(std::string tagStr);
}

#endif
