#include "string_utils.h"

std::string remove_prefix(const std::string &s, const std::string &prefix) {
    if (s.substr(0, prefix.length()) == prefix) {
        return s.substr(prefix.length());
    }
    return s;
}