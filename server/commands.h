#ifndef SERVER_SMART_TAG_COMMANDS_H
#define SERVER_SMART_TAG_COMMANDS_H

#include <string>

enum Commands {
    PING_SERVER,
    GET_TAGS_SCREENS,
    GET_TAGS_DATA,
    PUT_TAGS,
    SWITCH_TAG
};

namespace Command {
    std::string runCommand(const std::string& data);
}

#endif
