#ifndef SMART_PRICE_TAG_BOT_KEYBOARDS_H
#define SMART_PRICE_TAG_BOT_KEYBOARDS_H

#include <tgbot/tgbot.h>
#include <vector>
#include <string>
#include "botConsts.h"
#include "botParser.h"

namespace BotKeyboard {
    TgBot::KeyboardButton::Ptr keyboardBuilder(std::string text);
    TgBot::InlineKeyboardButton::Ptr inlineKeyboardBuilder(std::string text, std::string callbackData);
    TgBot::ReplyKeyboardMarkup::Ptr getReplayKeyboard(const std::vector<std::vector<std::string>> buttonsText);
    TgBot::InlineKeyboardMarkup::Ptr getInlineKeyboard(const std::vector<std::vector<std::string>> buttonsText, std::string getCallback(std::string));
}

#endif