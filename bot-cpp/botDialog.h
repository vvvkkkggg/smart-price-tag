#ifndef SMART_PRICE_TAG_BOT_DIALOG_H
#define SMART_PRICE_TAG_BOT_DIALOG_H
#include <sw/redis++/redis++.h>
#include <tgbot/tgbot.h>

enum UserStates {
    CREATE_USER,
    WAIT_HOST,
    WAIT_SECRET_KEY,
    WAIT_CONFIRMATION_SERVER_DATA,
    NORMAL_WORK
};

namespace BotDialog {
    bool dialogChainResponsibility(TgBot::Bot &bot, sw::redis::Redis &redis, const TgBot::Message::Ptr &message);
}

#endif
