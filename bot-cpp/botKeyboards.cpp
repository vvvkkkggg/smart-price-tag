#include "botKeyboards.h"

TgBot::KeyboardButton::Ptr BotKeyboard::keyboardBuilder(std::string text) {
    TgBot::KeyboardButton::Ptr button(new TgBot::KeyboardButton);
    button->text = text;
    return button;
}

TgBot::InlineKeyboardButton::Ptr BotKeyboard::inlineKeyboardBuilder(std::string text, std::string callbackData) {
    TgBot::InlineKeyboardButton::Ptr button(new TgBot::InlineKeyboardButton);
    button->text = text;
    button->callbackData = callbackData;
    return button;
}

TgBot::ReplyKeyboardMarkup::Ptr BotKeyboard::getReplayKeyboard(const std::vector<std::vector<std::string>> buttonsText) {
    TgBot::ReplyKeyboardMarkup::Ptr keyboard(new TgBot::ReplyKeyboardMarkup);

    std::for_each(
            buttonsText.begin(),
            buttonsText.end(),
            [&keyboard](std::vector<std::string> layer) {
                std::vector<TgBot::KeyboardButton::Ptr> buttons;

                std::for_each(
                        layer.begin(),
                        layer.end(),
                        [&buttons](auto &n) {
                            buttons.push_back(BotKeyboard::keyboardBuilder(n));
                        });

                keyboard->keyboard.push_back(buttons);
            }
    );

    return keyboard;
}

TgBot::InlineKeyboardMarkup::Ptr
BotKeyboard::getInlineKeyboard(const std::vector<std::vector<std::string>> buttonsText, std::string getCallback(std::string)) {
    TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);

    std::for_each(
            buttonsText.begin(),
            buttonsText.end(),
            [&keyboard, &getCallback](std::vector<std::string> layer) {
                std::vector<TgBot::InlineKeyboardButton::Ptr> buttons;

                std::for_each(layer.begin(), layer.end(), [&buttons, &getCallback](auto &n) {
                    buttons.push_back(BotKeyboard::inlineKeyboardBuilder(n, getCallback(n)));
                });

                keyboard->inlineKeyboard.push_back(buttons);
            });

    return keyboard;
}