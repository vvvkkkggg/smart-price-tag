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

    const std::vector<std::string> menuButtonsName = {SHOW_TAGS_NUMBERS, DUMP_TAGS, SET_TAGS, UPLOAD_TAGS};

    // Answers to menu buttons

    const std::string showTagsNumbersText(const std::vector<int> tagsIds) {
        std::ostringstream oss;
        if (!tagsIds.empty()) {
            std::copy(
                    tagsIds.begin(),
                    tagsIds.end() - 1,
                    std::ostream_iterator<int>(oss, ", ")
            );
            oss << tagsIds.back();
        }

        return "Установили на контроллерах режим настройки, на экранах должны высветиться номера.\n\n"
               "Сейчас Вам доступны экраны: " + oss.str();
    }

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

    const std::vector<std::string> inlineButtonsForDump = {DUMP_HTML, DUMP_CSV, DUMP_GOOGLE_TABLE};

    const std::string DUMP_KEY = "dump";
    const std::string EMPTY_DATA = "";

    auto DUMP_SUCCESSFUL_MESSAGE = [](const std::string s) {
        return "Начали подготавливать ценники в формате '" + s +"', скоро пришлем их Вам!";
    };
}