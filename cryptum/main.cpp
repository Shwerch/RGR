#include <chrono>
#include <iostream>
#include <vector>

#include "../utils/filesystem.h"
#include "../utils/hex_utils.h"
#include "../utils/parser.h"
#include "../utils/library_loader.h"

int main(int argc, char** argv) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    std::vector<std::string> args = {argv, argv + argc};
    Algorithm algorithm;
    size_t key_size;
    Mode mode;
    std::vector<uint8_t> key;
    std::vector<uint8_t> input;
    ParsedOutput output;
    try {
        if (parse_help(args)) return 0;
        algorithm = parse_algorithm(args);
        key_size = algorithm == Algorithm::Aes ? 32 : algorithm == Algorithm::Des ? 8 : 32;
        mode = parse_mode(args);
        key = parse_key(args, key_size);
        parse_save_key(args, key);
        input = parse_input(args);
        output = parse_output(args);
    } catch (const std::exception &e) {
        std::cerr << "arguments parsing error: " << e.what() << std::endl;
        return 1;
    }

    auto library = Library("aes");
    auto function = library.get_function("encrypt");
    auto data = function(input.data(), input.size(), key.data());
    std::vector<uint8_t> result(data, data + input.size());

    switch (output.mode) {
        case Output::Binary: {
            std::cout << "[";
            for (size_t i = 0; i < result.size(); i++) {
                std::cout << result[i];
                if (i == result.size() - 1) std::cout << " ";
            }
            std::cout << "]" << std::endl;
            break;
        }
        case Output::File: {
            write_bytes(output.path, result);
            break;
        }
        case Output::Hex: {
            std::cout << "0x" << vector_to_hex_str(result) << std::endl;
            break;
        }
        case Output::Text: {
            const std::string str(result.begin(), result.end());
            std::cout << str << std::endl;
            break;
        }
    }
    return 0;
}