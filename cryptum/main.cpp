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

    const auto library = Library("aes");
    const auto function = library.get_function("encrypt");
    const auto result = function(plaintext, 100, key);

    std::cout << (plaintext[0] == result[0]) << std::endl;
    return 0;
}