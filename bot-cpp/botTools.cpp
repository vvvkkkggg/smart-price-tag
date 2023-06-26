#include <sstream>
#include <string>
#include <vector>
#include "botConsts.h"
#include "botTools.h"

void BotTool::sendMessage(TgBot::Bot &bot, boost::variant<std::int64_t, std::string> chatId,
                          const std::string &text,
                          TgBot::GenericReply::Ptr replyMarkup) {
    bot.getApi().sendMessage(chatId, text, false, 0, replyMarkup, "Markdown");
}

std::vector<std::string> BotTool::split(const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}

template<typename T>
std::string BotTool::joinVector(const std::vector<T> elements, const std::string delim) {
    std::ostringstream oss;
    if (!elements.empty()) {
        std::copy(
                elements.begin(),
                elements.end(),
                std::ostream_iterator<T>(oss, delim.c_str())
        );
        oss << elements.back();
    }
    return oss.str();
}

const std::string BotConst::showTagsNumbersText(const std::vector<int> tagsIds) {
    return BotConst::SHOW_TAGS_NUMBERS_TEXT + BotTool::joinVector(tagsIds, " ");
}