#include <string>
#include <tgbot/tgbot.h>

namespace CallbackParser {
    std::string dumpInlineDumpButtonCallbackData(std::string callbackData) {
        return callbackData;
    }

    std::string loadInlineDumpButtonCallbackData(std::string callbackData) {
        if (StringTools::startsWith(callbackData, "dump_")) {
//        std::vector<std::string> dest; StringTools::split(callbackData, "dump_", &dest);
        }
        return callbackData;
    }
}