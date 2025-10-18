#include "io_utils.h"
#include "hex_utils.h"
#include "exception.h"
#include <iostream>
#include <string>

std::vector<uint8_t> input_bytes(size_t n) {
    std::cout << "Введите строку или hex (0x...): ";
    std::string input;
    std::getline(std::cin, input);

    if (input.substr(0, 2) == "0x" || input.substr(0, 2) == "0X") {
        const std::string hex_str = input.substr(2);
        bool is_hex = true;
        for (const char c : hex_str) {
            if (!std::isxdigit(c) && !std::isspace(c)) {
                is_hex = false;
                break;
            }
        }
        if (is_hex) {
            return parse_hex_to_vec(hex_str, n);
        }
    }

    std::vector<uint8_t> bytes(input.begin(), input.end());
    if (bytes.size() > n) {
        bytes.resize(n);
    } else if (bytes.size() < n) {
        throw EXCEPTION("Введено " + std::to_string(bytes.size()) + 
                       " байт, требуется " + std::to_string(n));
    }

    return bytes;
}

std::vector<uint8_t> input_all_bytes() {
    std::cout << "Введите строку или hex (0x...): ";
    std::string input;
    std::getline(std::cin, input);

    if (input.substr(0, 2) == "0x" || input.substr(0, 2) == "0X") {
        const std::string hex_str = input.substr(2);
        bool is_hex = true;
        for (const char c : hex_str) {
            if (!std::isxdigit(c) && !std::isspace(c)) {
                is_hex = false;
                break;
            }
        }
        if (is_hex) {
            return parse_hex_all(hex_str);
        }
    }

    return std::vector<uint8_t>(input.begin(), input.end());
}