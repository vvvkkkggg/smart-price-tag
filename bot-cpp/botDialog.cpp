#include "botDialog.h"
#include "botParser.h"
#include "botConsts.h"
#include "botTools.h"
#include "botDB.h"
#include "botKeyboards.h"

void userCreationDialog(TgBot::Bot &bot, sw::redis::Redis &redis, const TgBot::Message::Ptr &message) {
    BotTool::sendMessage(bot, message->chat->id, "приятно бла-бла, отправь сервер для начала");
    BotDB::updateUserNode(
            redis,
            message->from->id,
            BotDB::createUser(
                    redis,
                    message->from->id,
                    BotStructure::createUser(BotConst::EMPTY_DATA, BotConst::EMPTY_DATA, 0)
            ), UserStates::WAIT_HOST);
}

void userServerDialog(TgBot::Bot &bot, sw::redis::Redis &redis, const TgBot::Message::Ptr &message, int userId,
                      struct User *user) {
    if (user->node == UserStates::WAIT_HOST) {
        BotTool::sendMessage(bot, userId, "сохранили хост, давай ключ");
        BotDB::updateUserHost(redis, userId, user, message->text);
        BotDB::updateUserNode(redis, userId, user, UserStates::WAIT_SECRET_KEY);
        return;
    }

    if (user->node == UserStates::WAIT_SECRET_KEY) {
        TgBot::InlineKeyboardMarkup::Ptr inlineSetKeyboard = BotKeyboard::getInlineKeyboard(
                BotConst::inlineButtonsForSetConfirmation,
                CallbackParser::dumpInlineServerDataCallbackButton
        );

        BotTool::sendMessage(bot, userId,
                             "сохранили ключ, давай сверим, все ли верно у нас получилось? если не так введи ",
                             inlineSetKeyboard);
        BotDB::updateUserSecretKey(redis, userId, user, message->text);
        BotDB::updateUserNode(redis, userId, user, UserStates::WAIT_CONFIRMATION_SERVER_DATA);
        return;
    }

    BotTool::sendMessage(bot, userId,
                         "Подтверди или опровергни введенные тобой данные, чтобы продолжить дальше!");
}

bool isWaitUserServerData(int node) {
    return node == UserStates::WAIT_HOST || node == UserStates::WAIT_SECRET_KEY ||
           node == UserStates::WAIT_CONFIRMATION_SERVER_DATA;
}

#include <cstdio>

bool
BotDialog::dialogChainResponsibility(TgBot::Bot &bot, sw::redis::Redis &redis, const TgBot::Message::Ptr &message) {
    printf("tyt\n!");
    int userId = message->from->id;
    struct User *user = BotDB::getUser(redis, userId);

    if (!user) {
        userCreationDialog(bot, redis, message);
        return true;
    }

    if (isWaitUserServerData(user->node)) {
        userServerDialog(bot, redis, message, userId, user);
        return true;
    }

    return false;
}
