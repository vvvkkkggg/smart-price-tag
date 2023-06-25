#ifndef SMART_PRICE_TAG_BOT_CONSTS_H
#define SMART_PRICE_TAG_BOT_CONSTS_H

#include <vector>
#include <string>
#include <sstream>
#include <iterator>

namespace BotConst {
    // Menu buttons

    const std::string SHOW_TAGS_NUMBERS = "Вывести номера ценников";
    const std::string DUMP_TAGS = "Выгрузить ценники";
    const std::string SET_TAGS = "Установить ценник";
    const std::string UPLOAD_TAGS = "Загрузить ценники";

    const std::vector<std::vector<std::string>> menuButtonsName = {{SHOW_TAGS_NUMBERS}, {DUMP_TAGS}, {SET_TAGS}, {UPLOAD_TAGS}};

    // Answers to menu buttons

    const std::string showTagsNumbersText(const std::vector<int> tagsIds);

    const std::string SHOW_TAGS_NUMBERS_TEXT = "Установили на контроллерах режим настройки, на экранах должны высветиться номера.\n\n"
                                               "Сейчас Вам доступны экраны: ";

    const std::string DUMP_TAGS_TEXT = "В каком формате выгрузить ценники?";

    const std::string SET_TAGS_TEXT = "Напишите команду, где вместо [[...]] вставьте нужную информацию:\n\n"
                                      "`/set [номер экрана] [цена] [название]`\n\n"
                                      "Например установим на первый экран молоко от ВШЭ с ценой 250:\n\n"
                                      "`/set 1 250 Молоко ВШЭ`";

    const std::string UPLOAD_TAGS_TEXT = "Отправьте в чат Ваш csv-файл с колонками: экран, цена, название.\n\n"
                                         "Бот обработает таблицу, а затем запросит подтверждение на загрузку.";

    // Inline buttons for dump

    const std::string DUMP_HTML = "HTML";
    const std::string DUMP_CSV = "CSV";
    const std::string DUMP_GOOGLE_TABLE = "Google Table";

    const std::vector<std::vector<std::string>> inlineButtonsForDump = {{DUMP_HTML, DUMP_CSV, DUMP_GOOGLE_TABLE}};

    const std::string DUMP_KEY = "dump_";
    const std::string EMPTY_DATA = "";

//    auto DUMP_SUCCESSFUL_MESSAGE = [](const std::string s) {
//        return "Начали подготавливать ценники в формате '" + s + "', скоро пришлем их Вам!";
//    };

    // Inline buttons for set tag

    const std::string SET_YES = "Да!";
    const std::string SET_NO = "Нет";

    const std::vector<std::vector<std::string>> inlineButtonsForSetConfirmation = {{SET_YES, SET_NO}};

    const std::string SET_KEY = "set_";
}

#endif