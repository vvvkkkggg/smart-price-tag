#include <cstdio>
#include <fstream>
#include "json.hpp"
#include "const.h"
#include <tgbot/tgbot.h>
#include <vector>
#include <string>

TgBot::KeyboardButton::Ptr keyboardBuilder(std::string text) {
    TgBot::KeyboardButton::Ptr button(new TgBot::KeyboardButton);
    button->text = text;
    return button;
}

int main() {
    TgBot::Bot bot("");

    TgBot::ReplyKeyboardMarkup::Ptr menuKeyboard(new TgBot::ReplyKeyboardMarkup);
    std::vector<TgBot::KeyboardButton::Ptr> buttons;

    std::for_each(
            BotConst::menuButtonsName.begin(),
            BotConst::menuButtonsName.end(),
            [&menuKeyboard](auto &n) {
                std::vector<TgBot::KeyboardButton::Ptr> buttons;
                buttons.push_back(keyboardBuilder(n));
                menuKeyboard->keyboard.push_back(buttons);
            }
    );

    bot.getEvents().onCommand("start", [&bot, &menuKeyboard](const TgBot::Message::Ptr &message) {
        bot.getApi().sendMessage(message->chat->id, message->text, false, 0, menuKeyboard);
    });

    bot.getEvents().onAnyMessage([&bot](const TgBot::Message::Ptr &message) {
        printf("User wrote %s\n", message->text.c_str());
//
        if (StringTools::startsWith(message->text, BotConst::SHOW_TAGS_NUMBERS)) {
            bot.getApi().sendMessage(message->chat->id, "pohui: ");
        } else if (StringTools::startsWith(message->text, BotConst::DUMP_TAGS)) {
            bot.getApi().sendMessage(message->chat->id, "pohui dump ");
        } else if (StringTools::startsWith(message->text, BotConst::SET_TAGS)) {
            bot.getApi().sendMessage(message->chat->id,
                                     "Напишите в команду, где вместо [[...]] укажите нужную информацию:"
                                     "\n\n`/set [номер экрана] [цена] [название]`\n\n"
                                     "Например установим на первый экран молоко с ценой 250:\n\n"
                                     "`/set 1 250 Молоко`", false, 0, nullptr, "Markdown");
        } else if (StringTools::startsWith(message->text, BotConst::UPLOAD_TAGS)) {
            bot.getApi().sendMessage(message->chat->id, "Отправьте Ваш csv-файл в чат.", false, 0, nullptr, "Markdown");
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