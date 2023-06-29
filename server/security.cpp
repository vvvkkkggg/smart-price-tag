#include "security.h"

bool Security::isSecretKeyValid(const std::string& userSecretKey) {
    return "ok" == userSecretKey;
}