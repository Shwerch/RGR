#include "hex_utils.h"
#include "exception.h"
#include <sstream>
#include <iomanip>

std::vector<uint8_t> parse_hex_to_vec(const std::string &hex_str, size_t n) {
    std::string hex_clean;
    for (char c : hex_str) {
        if (!std::isspace(c)) {
            hex_clean += c;
        }
    }

    if (hex_clean.length() % 2 != 0) {
        throw EXCEPTION("Hex строка должна содержать четное количество символов");
    }

    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < hex_clean.length(); i += 2) {
        std::string byte_str = hex_clean.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(std::stoul(byte_str, nullptr, 16));
        bytes.push_back(byte);
    }

    if (bytes.size() < n) {
        throw EXCEPTION("Hex содержит " + std::to_string(bytes.size()) + 
                       " байт, требуется " + std::to_string(n));
    }

    bytes.resize(n);
    return bytes;
}

std::vector<uint8_t> parse_hex_all(const std::string &hex_str) {
    std::string hex_clean;
    std::string input = hex_str;
    
    if (input.substr(0, 2) == "0x" || input.substr(0, 2) == "0X") {
        input = input.substr(2);
    }

    for (char c : input) {
        if (!std::isspace(c)) {
            hex_clean += c;
        }
    }

    if (hex_clean.length() % 2 != 0) {
        throw EXCEPTION("Hex строка должна содержать четное количество символов");
    }

    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < hex_clean.length(); i += 2) {
        std::string byte_str = hex_clean.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(std::stoul(byte_str, nullptr, 16));
        bytes.push_back(byte);
    }

    return bytes;
}

std::string vector_to_hex_str(const std::vector<uint8_t> &bytes) {
    std::ostringstream oss;
    oss << "0x";
    if (bytes.empty()) {
        oss << "00";
    } else {
        for (uint8_t byte : bytes) {
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
        }
    }
    return oss.str();
}