#include "parser.h"

#include <iostream>

int main(int argc, const char** argv) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    try {

        Arguments arguments = parse_arguments(argc, argv);
        check_arguments(arguments);
        std::vector<uint8_t> key = get_key(arguments);
        export_key(arguments, key);
        std::vector<uint8_t> input = get_input(arguments);
        // output = operation
        // export output

        std::cout << "input: ";
        for (int i : input) {
            std::cout << (unsigned int) i << " ";
        }
        std::cout << std::endl;

    } catch (const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(1);
    }

    return 0;
}