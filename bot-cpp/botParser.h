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

    std::string dumpInlineServerDataCallbackButton(std::string setStr);

    std::string *loadInlineServerDataCallbackButton(std::string setStr);
}

namespace CommandParser {
    Tag *loadTagFromSetCommand(std::string tagStr);
}

namespace JsonParser {
    std::string dumpUserToString(User *user);

    struct User *loadUserFromStringJson(std::string j);
};

#endif
