#include <iostream>
#include <vector>

#include "../utils/parser.h"
#include "../utils/library_loader.h"

int main(const int argc, const char** argv) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    const std::vector<std::string> args = {argv, argv + argc};
    if (parse_help(args)) return 0;

    const auto plaintext = new uint8_t[100];
    const auto key = new uint8_t[32];

    const uint8_t* result;
    try {
        const auto library = DynamicLibrary("aes", "encrypt");
        result = library.function(plaintext, 100, key);
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    std::cout << (plaintext[0] == result[0]) << std::endl;
    return 0;
}