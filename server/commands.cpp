#include <unordered_map>
#include "json.hpp"
#include "commands.h"
#include "db.h"

nlohmann::json pingServer(const nlohmann::json &_) {
    return nlohmann::json{{"status", 200}};
}

void to_json(nlohmann::json& j, const Entity& e) {
    j = {{"screenId", e.screenId}, {"productName", e.productName}, {"cost", e.cost}};
}

nlohmann::json getAllScreens(const nlohmann::json &_) {
    nlohmann::json j;
    j["screens"] = {1};
    j["status"] = 200;
    return j;
}

nlohmann::json getAllTags(const nlohmann::json &_) {
    std::vector<Entity> entities = DB::getAllEntities();
    nlohmann::json j;
    j["tags"] = entities;
    j["status"] = 200;
    return j;
}

#include <iostream>

nlohmann::json putTags(const nlohmann::json &jsonData) {
    std::cout << jsonData["tags"];
    return nlohmann::json{{"status", 200}};
}

nlohmann::json switchTag(const nlohmann::json &jsonData) {
    std::string tagId = jsonData["tagId"];

    return nlohmann::json{{"status", 200}, {"switch", 0}};
}

std::string Command::runCommand(const std::string &data) {
    nlohmann::json jsonData = nlohmann::json::parse(data);

    std::unordered_map<int, std::function<nlohmann::json(const nlohmann::json &)>> commands {
            {Commands::GET_SERVER_STATUS, pingServer},
            {Commands::GET_TAGS_SCREENS, getAllScreens},
            {Commands::GET_TAGS_DATA,     getAllTags},
            {Commands::PUT_TAGS,          putTags},
            {Commands::SWITCH_TAG,        switchTag},
    };

    int command = std::stoi(to_string(jsonData["command"]));
    auto func = commands.find(command);

    if (func == commands.end()) {
        return nlohmann::json{{"status", 404}}.dump();
    }

    return func->second(jsonData);
}