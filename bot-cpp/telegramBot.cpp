#include <cstdio>
#include <tgbot/tgbot.h>
#include <string>
#include <vector>
#include "json.hpp"
#include "botConsts.h"
#include "botKeyboards.h"
#include "botParser.h"
#include "botTools.h"

// бд (с регой для сервера); второй поток, который обрабатывает запросы; csv парсер (видимо уже на сервере, чтобы не грузится)

int main() {
    TgBot::Bot bot("6144696073:AAERZzFet5rgILChIMfz_aEiQ05nXDI6Ab4");

    TgBot::ReplyKeyboardMarkup::Ptr menuKeyboard = BotKeyboard::getReplayKeyboard(BotConst::menuButtonsName);
    TgBot::InlineKeyboardMarkup::Ptr inlineDumpKeyboard = BotKeyboard::getInlineKeyboard(
            BotConst::inlineButtonsForDump, CallbackParser::dumpInlineDumpCallbackButton
    );

    bot.getEvents().onCommand("start", [&bot, &menuKeyboard](const TgBot::Message::Ptr &message) {
        BotTool::sendMessage(bot, message->chat->id, "start???", menuKeyboard);
    });

    bot.getEvents().onCommand("set", [&bot](const TgBot::Message::Ptr &message) {
        Tag *tag = CommandParser::loadTagFromSetCommand(message->text);

        if (tag == nullptr) {
            bot.getApi().sendMessage(message->chat->id, BotConst::SET_ERROR_MESSAGE);
            return;
        }

        /* Тут грузим/перезаписываем в базу, получаем id */
        int x = 1;

        TgBot::InlineKeyboardMarkup::Ptr inlineSetKeyboard = BotKeyboard::getInlineKeyboard(
                BotConst::inlineButtonsForSetConfirmation,
                CallbackParser::dumpInlineSetCallbackButton,
                CallbackParser::dumpInlineSetTagIdCallbackButton,
                x
        );

        std::ostringstream formatResult;
        formatResult << boost::format(BotConst::SET_CONFORMATION_MESSAGE) % tag->screenId % tag->name % tag->price;

        BotTool::sendMessage(bot, message->chat->id, formatResult.str(), inlineSetKeyboard);
    });

    bot.getEvents().onCommand("switch", [&bot](const TgBot::Message::Ptr &message) {
        bot.getApi().sendMessage(message->chat->id, "поменял режим, потом реальный ответ с сервера будет");
    });

    bot.getEvents().onCommand("drop", [&bot](const TgBot::Message::Ptr &message) {
        bot.getApi().sendMessage(message->chat->id, "удалили сервер из бедешки");
    });

    bot.getEvents().onUnknownCommand([&bot, &menuKeyboard](const TgBot::Message::Ptr &message) {
        bot.getApi().sendMessage(message->chat->id,
                                 "Неизвестная команда, введите `/`, чтобы получить список доступных команд.", false, 0,
                                 menuKeyboard, "Markdown");
    });

    bot.getEvents().onCallbackQuery([&bot](TgBot::CallbackQuery::Ptr callbackQuery) {
        std::string *dumpTags = CallbackParser::loadInlineDumpCallbackButton(callbackQuery->data);
        if (dumpTags != nullptr) {
            std::ostringstream formatResult;
            formatResult << boost::format(BotConst::DUMP_SUCCESSFUL_MESSAGE) % *dumpTags;

            bot.getApi().answerCallbackQuery(callbackQuery->id, BotConst::SUCCESS_MESSAGE);
            bot.getApi().editMessageText(
                    formatResult.str(), callbackQuery->message->chat->id, callbackQuery->message->messageId
            );

            /* add to queue */
            return;
        }

        struct TagConfirmation *tagConfirmation = CallbackParser::loadInlineSetCallbackButton(callbackQuery->data);
        if (tagConfirmation != nullptr) {
            // check yes/no answer for confirmation
            
            std::ostringstream result;
            result << boost::format(BotConst::SET_SUCCESSFUL_MESSAGE) % tagConfirmation->id % "fsdfds" % 1;

            bot.getApi().answerCallbackQuery(callbackQuery->id, BotConst::SUCCESS_MESSAGE);
            bot.getApi().editMessageText(result.str(), callbackQuery->message->chat->id,
                                         callbackQuery->message->messageId);

            return;
        }

        bot.getApi().sendMessage(callbackQuery->message->chat->id, "hui");
    });

    bot.getEvents().onAnyMessage([&bot, &inlineDumpKeyboard](const TgBot::Message::Ptr &message) {
        /* Check full user registration */

        if (StringTools::startsWith(message->text, BotConst::SHOW_TAGS_NUMBERS)) {
            /* Send data to server */
            std::vector<int> v;
            BotTool::sendMessage(bot, message->chat->id, BotConst::showTagsNumbersText(v));
        } else if (StringTools::startsWith(message->text, BotConst::DUMP_TAGS)) {
            BotTool::sendMessage(bot, message->chat->id, BotConst::DUMP_TAGS_TEXT, inlineDumpKeyboard);
        } else if (StringTools::startsWith(message->text, BotConst::SET_TAGS)) {
            BotTool::sendMessage(bot, message->chat->id, BotConst::SET_TAGS_TEXT);
        } else if (StringTools::startsWith(message->text, BotConst::UPLOAD_TAGS)) {
            BotTool::sendMessage(bot, message->chat->id, BotConst::UPLOAD_TAGS_TEXT);
        } else if (StringTools::startsWith(message->text, "/")) {
            // что-то тут сделаю
            BotTool::sendMessage(bot, message->chat->id, "Your message is: " + message->text);
        }
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