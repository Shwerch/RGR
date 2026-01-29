#include "parser.h"

int main(int argc, const char** argv) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    auto arguments = parse_arguments(argc, argv);
    check_arguments(arguments);
    auto key = get_key(arguments);
    export_key(arguments, key);
    auto input = get_input(arguments);

    return 0;
}