#ifndef SMART_PRICE_TAG_BOT_TOOLS_H
#define SMART_PRICE_TAG_BOT_TOOLS_H
#include <tgbot/tgbot.h>
#include <vector>
#include <string>
#include "json.hpp"

namespace BotTool {
    void sendMessage(TgBot::Bot &bot, boost::variant<std::int64_t, std::string> chatId,
                     const std::string &text,
                     TgBot::GenericReply::Ptr replyMarkup = nullptr);
    std::vector<std::string> split (const std::string &s, char delim);
    template <typename T> std::string joinVector(const std::vector<T> elements, const std::string delim);
}

#endif
