#include <cstdio>
#include <tgbot/tgbot.h>
#include <sw/redis++/redis++.h>
#include "botHandlers.h"

// бд (с регой для сервера); второй поток, который обрабатывает запросы; csv парсер (видимо уже на сервере, чтобы не грузится)

int main() {
    TgBot::Bot bot("");
    BotHandler::initBotHandlers(bot);

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