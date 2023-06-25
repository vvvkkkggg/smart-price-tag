#include <cstdio>
#include "json.hpp"

#include "botConsts.h"
#include "botKeyboards.h"
#include "botParser.cpp"

#include <tgbot/tgbot.h>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <string>

int main() {
    TgBot::Bot bot("6144696073:AAERZzFet5rgILChIMfz_aEiQ05nXDI6Ab4");

    TgBot::InlineKeyboardMarkup::Ptr inlineDumpKeyboard(new TgBot::InlineKeyboardMarkup);
    std::vector<TgBot::InlineKeyboardButton::Ptr> inlineDumpButtons;

    std::for_each(
            BotConst::inlineButtonsForDump.begin(),
            BotConst::inlineButtonsForDump.end(),
            [&inlineDumpButtons](auto &n) {
                // std::string s = dumpInlineDumpButtonCallbackData(n);
                inlineDumpButtons.push_back(BotKeyboard::inlineKeyboardBuilder(
                        n, n));
            });

    inlineDumpKeyboard->inlineKeyboard.push_back(inlineDumpButtons);

    bot.getEvents().onCommand("start", [&bot](const TgBot::Message::Ptr &message) {
        bot.getApi().sendMessage(message->chat->id, message->text, false, 0, BotKeyboard::menuKeyboard);
    });

    bot.getEvents().onCallbackQuery([&bot](TgBot::CallbackQuery::Ptr callbackQuery) {

        std::string dumpTags = CallbackParser::loadInlineDumpButtonCallbackData(callbackQuery->data);
        if (dumpTags != BotConst::EMPTY_DATA) {
            bot.getApi().answerCallbackQuery(callbackQuery->id, "Успешный выбор!");
            bot.getApi().editMessageText(BotConst::DUMP_SUCCESSFUL_MESSAGE(dumpTags), callbackQuery->message->chat->id, callbackQuery->message->messageId);
            // add to queue to load
//            bot.getApi().sendMessage(callbackQuery->message->chat->id, BotConst::DUMP_SUCCESSFUL_MESSAGE(dumpTags));
            return ;
        }

        bot.getApi().sendMessage(callbackQuery->message->chat->id, "hui");
    });


    bot.getEvents().onAnyMessage([&bot, &inlineDumpKeyboard](const TgBot::Message::Ptr &message) {
        printf("User wrote %s\n", message->text.c_str());
//
        if (StringTools::startsWith(message->text, BotConst::SHOW_TAGS_NUMBERS)) {
            std::vector<int> v;
            bot.getApi().sendMessage(message->chat->id, BotConst::showTagsNumbersText(v));
        } else if (StringTools::startsWith(message->text, BotConst::DUMP_TAGS)) {
            bot.getApi().sendMessage(message->chat->id, BotConst::DUMP_TAGS_TEXT, false, 0, inlineDumpKeyboard);
        } else if (StringTools::startsWith(message->text, BotConst::SET_TAGS)) {
            bot.getApi().sendMessage(message->chat->id,BotConst::SET_TAGS_TEXT, false, 0, nullptr, "Markdown");
        } else if (StringTools::startsWith(message->text, BotConst::UPLOAD_TAGS)) {
            bot.getApi().sendMessage(message->chat->id, BotConst::UPLOAD_TAGS_TEXT, false, 0, nullptr, "Markdown");
        } else
            bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text);
    });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (TgBot::TgException &e) {
        printf("error: %s\n", e.what());
    }
    return 0;
}