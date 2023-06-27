#include <unordered_map>
#include "commands.h"
#include "json.hpp"

nlohmann::json pingServer(const nlohmann::json &jsonData) {
    return nlohmann::json{{"status", 200}};
}

std::string Command::runCommand(const std::string &data) {
    nlohmann::json jsonData = nlohmann::json::parse(data);

    std::unordered_map<int, std::function<nlohmann::json(const nlohmann::json &)>> commands {
            {Commands::PING_SERVER, pingServer},
            {Commands::GET_TAGS_SCREENS, nullptr},
            {Commands::GET_TAGS_DATA, nullptr},
            {Commands::PUT_TAGS, nullptr},
            {Commands::SWITCH_TAG, nullptr},
    };

    int command = std::stoi(to_string(jsonData["command"]));
    auto func = commands.find(command);

    if (func == commands.end()) {
        return nlohmann::json{{"status", 404}}.dump();
    }

    return func->second(jsonData);
}