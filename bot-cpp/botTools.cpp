#include <sstream>
#include <string>
#include <vector>
#include "botConsts.h"
#include "botTools.h"

std::vector<std::string> BotTool::split (const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

const std::string BotConst::showTagsNumbersText(const std::vector<int> tagsIds) {
    std::ostringstream oss;
    if (!tagsIds.empty()) {
        std::copy(
                tagsIds.begin(),
                tagsIds.end() - 1,
                std::ostream_iterator<int>(oss, ", ")
        );
        oss << tagsIds.back();
    }

    return BotConst::SHOW_TAGS_NUMBERS_TEXT + oss.str();
}