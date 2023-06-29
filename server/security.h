#ifndef SERVER_SMART_TAG_SECURITY_H
#define SERVER_SMART_TAG_SECURITY_H

#include <string>

namespace Security {
    bool isSecretKeyValid(const std::string& userSecretKey);
}

#endif
