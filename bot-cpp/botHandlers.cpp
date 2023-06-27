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

void addStartCommandHandler(TgBot::Bot &bot) {

    bot.getEvents().onCommand("start", [&bot](const TgBot::Message::Ptr &message) {
        if (BotDialog::dialogChainResponsibility(bot, redis, message)) {
            return ;
        }
        BotTool::sendMessage(bot, message->chat->id, "start???", menuKeyboard);
    });

}

void addSetCommandHandler(TgBot::Bot &bot) {
    bot.getEvents().onCommand("set", [&bot](const TgBot::Message::Ptr &message) {
        if (BotDialog::dialogChainResponsibility(bot, redis, message)) {
            return ;
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
                CallbackParser::dumpInlineSetTagIdCallbackButton,
                x
        );

        std::ostringstream formatResult;
        formatResult << boost::format(BotConst::SET_CONFORMATION_MESSAGE) % tag->screenId % tag->name % tag->price;

        BotTool::sendMessage(bot, message->chat->id, formatResult.str(), inlineSetKeyboard);
    });

}

void addSwitchCommandHandler(TgBot::Bot &bot) {
    bot.getEvents().onCommand("switch", [&bot](const TgBot::Message::Ptr &message) {
        if (BotDialog::dialogChainResponsibility(bot, redis, message)) {
            return ;
        }
        bot.getApi().sendMessage(message->chat->id, "поменял режим, потом реальный ответ с сервера будет");
    });
}

void addDropCommandHandler(TgBot::Bot &bot) {
    bot.getEvents().onCommand("drop", [&bot](const TgBot::Message::Ptr &message) {
        if (BotDialog::dialogChainResponsibility(bot, redis, message)) {
            return ;
        }

        bot.getApi().sendMessage(message->chat->id, "удалили сервер из бедешки");
    });
}

void addUnknownCommandHandler(TgBot::Bot &bot) {
    bot.getEvents().onUnknownCommand([&bot](const TgBot::Message::Ptr &message) {
        if (BotDialog::dialogChainResponsibility(bot, redis, message)) {
            return ;
        }

        bot.getApi().sendMessage(message->chat->id,
                                 "Неизвестная команда, введите `/`, чтобы получить список доступных команд.", false, 0,
                                 menuKeyboard, "Markdown");
    });
}

void addCallbackHandler(TgBot::Bot &bot) {
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

        std::string *serverDataConfirmation = CallbackParser::loadInlineServerDataCallbackButton(callbackQuery->data);
        if (serverDataConfirmation != nullptr) {
            if (*serverDataConfirmation == BotConst::SET_YES) {
                BotDB::updateUserNode(redis, callbackQuery->from->id, BotDB::getUser(redis, callbackQuery->from->id), UserStates::NORMAL_WORK);
                BotTool::sendMessage(bot, callbackQuery->message->chat->id, "сохранили, четко");
            } else {
                BotTool::sendMessage(bot, callbackQuery->message->chat->id, "жаль, давай снова, отправь хоста");
                BotDB::updateUserNode(redis, callbackQuery->from->id, BotDB::getUser(redis, callbackQuery->from->id), UserStates::WAIT_HOST);
            }

            return;
        }

        bot.getApi().sendMessage(callbackQuery->message->chat->id, "hui");
    });
}

void addAnyMessageHandler(TgBot::Bot &bot) {
    bot.getEvents().onAnyMessage([&bot](const TgBot::Message::Ptr &message) {
        if (BotDialog::dialogChainResponsibility(bot, redis, message)) {
            return ;
        }

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