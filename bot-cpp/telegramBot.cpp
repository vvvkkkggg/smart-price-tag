#include <cstdio>
#include <tgbot/tgbot.h>
#include <string>
#include <vector>
#include "json.hpp"
#include "botConsts.h"
#include "botKeyboards.h"
#include "botParser.h"


int main() {
    TgBot::Bot bot("6144696073:AAERZzFet5rgILChIMfz_aEiQ05nXDI6Ab4");

    TgBot::ReplyKeyboardMarkup::Ptr menuKeyboard = BotKeyboard::getReplayKeyboard(BotConst::menuButtonsName);
    TgBot::InlineKeyboardMarkup::Ptr inlineDumpKeyboard = BotKeyboard::getInlineKeyboard(
            BotConst::inlineButtonsForDump, CallbackParser::dumpInlineDumpCallbackButton
    );

    bot.getEvents().onCommand("start", [&bot, &menuKeyboard](const TgBot::Message::Ptr &message) {
        bot.getApi().sendMessage(message->chat->id, message->text, false, 0, menuKeyboard);
    });

    bot.getEvents().onCommand("set", [&bot](const TgBot::Message::Ptr &message) {
        Tag *tag = CommandParser::loadTagFromSetCommand(message->text);
        if (tag == nullptr) {
            bot.getApi().sendMessage(message->chat->id, "отправил не то, давай по новой");
            return ;
        }

        int x = 1;

        TgBot::InlineKeyboardMarkup::Ptr inlineSetKeyboard = BotKeyboard::getInlineKeyboard(
                BotConst::inlineButtonsForSetConfirmation,
                CallbackParser::dumpInlineSetCallbackButton,
                CallbackParser::dumpInlineSetTagIdCallbackButton,
                x
        );

        bot.getApi().sendMessage(message->chat->id,
                "Вы хотите на экран " +
                std::to_string(tag->screenId) +
                " выставить \"" +
                tag->name +
                "\" за " +
                std::to_string(tag->price) +
                " рублей.\n\n" +
                "Подтверждаете это действие?", false, 0, inlineSetKeyboard);
    });

    bot.getEvents().onCommand("switch", [&bot](const TgBot::Message::Ptr &message) {
        bot.getApi().sendMessage(message->chat->id, "поменял режим, потом реальный ответ с сервера будет");
    });

    bot.getEvents().onCommand("drop", [&bot](const TgBot::Message::Ptr &message) {
        bot.getApi().sendMessage(message->chat->id, "удалили сервер из бедешки");
    });

    bot.getEvents().onUnknownCommand([&bot, &menuKeyboard](const TgBot::Message::Ptr &message) {
        bot.getApi().sendMessage(message->chat->id, "Неизвестная команда, введите `/`, чтобы получить список доступных команд.", false, 0, menuKeyboard, "Markdown");
    });

    bot.getEvents().onCallbackQuery([&bot](TgBot::CallbackQuery::Ptr callbackQuery) {

        std::string *dumpTags = CallbackParser::loadInlineDumpCallbackButton(callbackQuery->data);
        if (dumpTags != nullptr) {
            bot.getApi().answerCallbackQuery(callbackQuery->id, "Успешный выбор!");
            bot.getApi().editMessageText(/*BotConst::DUMP_SUCCESSFUL_MESSAGE(dumpTags)*/ "fd", callbackQuery->message->chat->id, callbackQuery->message->messageId);
            // add to queue to load
//            bot.getApi().sendMessage(callbackQuery->message->chat->id, BotConst::DUMP_SUCCESSFUL_MESSAGE(dumpTags));
            return ;
        }

        struct TagConfirmation *tagConfirmation = CallbackParser::loadInlineSetCallbackButton(callbackQuery->data);
        if (tagConfirmation != nullptr) {
            bot.getApi().answerCallbackQuery(callbackQuery->id, "Отправили на сервер!");
            bot.getApi().editMessageText(/*BotConst::DUMP_SUCCESSFUL_MESSAGE(dumpTags)*/ "fd", callbackQuery->message->chat->id, callbackQuery->message->messageId);

            return;
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