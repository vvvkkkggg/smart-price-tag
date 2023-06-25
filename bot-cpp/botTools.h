#ifndef SMART_PRICE_TAG_BOT_TOOLS_H
#define SMART_PRICE_TAG_BOT_TOOLS_H
#include <vector>
#include <string>

namespace BotTool {
    std::vector<std::string> split (const std::string &s, char delim);
    template <typename T> std::string joinVector(const std::vector<T> elements, const std::string delim);
}
#endif
