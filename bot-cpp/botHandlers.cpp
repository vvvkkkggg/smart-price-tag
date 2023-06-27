#include "botHandlers.h"
#include "botKeyboards.h"
#include "botTools.h"
#include "botDB.h"
#include "botDialog.h"

TgBot::ReplyKeyboardMarkup::Ptr menuKeyboard = BotKeyboard::getReplayKeyboard(BotConst::menuButtonsName);
TgBot::InlineKeyboardMarkup::Ptr inlineDumpKeyboard = BotKeyboard::getInlineKeyboard(
        BotConst::inlineButtonsForDump, CallbackParser::dumpInlineDumpCallbackButton
);

auto redis = sw::redis::Redis("tcp://127.0.0.1:6379");

#include <cstdio>

void addStartCommandHandler(TgBot::Bot &bot) {
    bot.getEvents().onCommand("start", [&bot](const TgBot::Message::Ptr &message) {
        auto user = BotDB::getUser(redis, message->from->id);

        if (!user || user->node != UserStates::NORMAL_WORK) {
            BotDialog::dialogChainResponsibility(bot, redis, message);
            return;
        }

        std::string text = "Привет! Мы работаем, твой сервер, который ты указал в последний раз, имеет следующие данные:\n\n"
                           "*Сервер* - _%s_\n"
                           "*Секретный ключ* - _%s_\n\n"
                           "Если это неактуальные данные, то введите `/drop`.";

        std::ostringstream result;
        result << boost::format(text) % user->host % user->secretKey;

        BotTool::sendMessage(bot, message->chat->id, result.str(), menuKeyboard);
    });
}

void addSetCommandHandler(TgBot::Bot &bot) {
    bot.getEvents().onCommand("set", [&bot](const TgBot::Message::Ptr &message) {
        if (BotDialog::dialogChainResponsibility(bot, redis, message)) {
            return;
        }

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
                CallbackParser::dumpInlineSetTagIdCallbackButton, x
        );

        std::ostringstream formatResult;
        formatResult << boost::format(BotConst::SET_CONFORMATION_MESSAGE) % tag->screenId % tag->name % tag->price;

        BotTool::sendMessage(bot, message->chat->id, formatResult.str(), inlineSetKeyboard);
    });

}

void addSwitchCommandHandler(TgBot::Bot &bot) {
    bot.getEvents().onCommand("switch", [&bot](const TgBot::Message::Ptr &message) {
        if (BotDialog::dialogChainResponsibility(bot, redis, message)) {
            return;
        }

        bot.getApi().sendMessage(message->chat->id,
                                 "Поменяли режим где-то, потом реальный ответ с сервера будет!)");
    });
}

void addDropCommandHandler(TgBot::Bot &bot) {
    bot.getEvents().onCommand("drop", [&bot](const TgBot::Message::Ptr &message) {
//        if (BotDialog::dialogChainResponsibility(bot, redis, message)) {
//            return;
//        }

        int userId = message->from->id;
        BotDB::updateUserNode(redis, userId, BotDB::getUser(redis, userId), UserStates::WAIT_HOST);
        BotTool::sendMessage(bot, message->chat->id,
                             "Мы отвязали текущий сервер от Вашего аккаунта. "
                             "Если хотите добавить новый, просто пришлите URL сервера!");
    });
}

void addUnknownCommandHandler(TgBot::Bot &bot) {
    bot.getEvents().onUnknownCommand([&bot](const TgBot::Message::Ptr &message) {
        if (BotDialog::dialogChainResponsibility(bot, redis, message)) {
            return;
        }

        BotTool::sendMessage(bot, message->chat->id,
                             "Неизвестная команда, введите `/`, чтобы получить список доступных команд.", menuKeyboard);
    });
}

void addCallbackHandler(TgBot::Bot &bot) {
    bot.getEvents().onCallbackQuery([&bot](const TgBot::CallbackQuery::Ptr &callbackQuery) {

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
            std::ostringstream result;

            if (tagConfirmation->answer == BotConst::SET_YES) {
                result << boost::format(BotConst::SET_SUCCESSFUL_MESSAGE)
                          % tagConfirmation->id % "я ничо не сохранил" % 1;
            } else {
                result << boost::format(BotConst::SET_UNSUCCESSFUL_MESSAGE)
                          % tagConfirmation->id % "я ничо не сохранил" % 1;
            }

            bot.getApi().answerCallbackQuery(callbackQuery->id, BotConst::SUCCESS_MESSAGE);
            bot.getApi().editMessageText(result.str(), callbackQuery->message->chat->id,
                                         callbackQuery->message->messageId);

            return;
        }

        std::string *serverDataConfirmation = CallbackParser::loadInlineServerDataCallbackButton(callbackQuery->data);
        if (serverDataConfirmation != nullptr) {
            auto user = BotDB::getUser(redis, callbackQuery->from->id);
            std::string answer;

            if (*serverDataConfirmation == BotConst::SET_YES) {
                BotDB::updateUserNode(redis, callbackQuery->from->id, user, UserStates::NORMAL_WORK);
                answer = "Мы сохранили Ваши данные!";
            } else {
                BotDB::updateUserNode(redis, callbackQuery->from->id, user, UserStates::WAIT_HOST);
                answer = "Давайте начнем сначала, отправьте URL сервера!";
            }

            bot.getApi().editMessageText(answer, callbackQuery->message->chat->id,
                                         callbackQuery->message->messageId);
            bot.getApi().answerCallbackQuery(callbackQuery->id, BotConst::SUCCESS_MESSAGE);

            return;
        }
    });
}

void addAnyMessageHandler(TgBot::Bot &bot) {
    bot.getEvents().onAnyMessage([&bot](const TgBot::Message::Ptr &message) {
        if (StringTools::startsWith(message->text, "/")) {
            return ;
        }

        if (BotDialog::dialogChainResponsibility(bot, redis, message)) {
            return;
        }

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
        } else {
            BotTool::sendMessage(bot, message->chat->id,
                                 "Мы стараемся не отвечать на неизвестный текст, поэтому как-то так..");
        }
    });
}

void BotHandler::initBotHandlers(TgBot::Bot &bot) {
    addStartCommandHandler(bot);
    addSetCommandHandler(bot);
    addSwitchCommandHandler(bot);
    addDropCommandHandler(bot);
    addUnknownCommandHandler(bot);
    addCallbackHandler(bot);
    addAnyMessageHandler(bot);
}