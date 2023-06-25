#include <tgbot/tgbot.h>
#include "botConsts.h"

namespace BotKeyboard {
    TgBot::KeyboardButton::Ptr keyboardBuilder(std::string text) {
        TgBot::KeyboardButton::Ptr button(new TgBot::KeyboardButton);
        button->text = text;
        return button;
    }

    TgBot::InlineKeyboardButton::Ptr inlineKeyboardBuilder(std::string text, std::string callbackData) {
        TgBot::InlineKeyboardButton::Ptr button(new TgBot::InlineKeyboardButton);
        button->text = text;
        button->callbackData = callbackData;
        return button;
    }

    TgBot::ReplyKeyboardMarkup::Ptr getMenuKeyboard() {
        TgBot::ReplyKeyboardMarkup::Ptr menuKeyboard(new TgBot::ReplyKeyboardMarkup);

        std::for_each(
                BotConst::menuButtonsName.begin(),
                BotConst::menuButtonsName.end(),
                [&menuKeyboard](auto &n) {
                    std::vector<TgBot::KeyboardButton::Ptr> buttons;
                    buttons.push_back(BotKeyboard::keyboardBuilder(n));
                    menuKeyboard->keyboard.push_back(buttons);
                }
        );

        return menuKeyboard;
    }

    TgBot::ReplyKeyboardMarkup::Ptr menuKeyboard = BotKeyboard::getMenuKeyboard();
}