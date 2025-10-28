#include <iostream>
#include <filesystem>

#include "parser.h"
#include "string_utils.h"
#include "rand_utils.h"
#include "library_loader.h"
#include "filesystem.h"
#include "hex_utils.h"
#include "io_utils.h"

bool has_arg(const std::vector<std::string> &args, const std::string &long_name, 
             const std::string &short_name) {
    for (size_t i = 1; i < args.size(); ++i) {
        if (args[i].find(long_name) == 0 || args[i].find(short_name) == 0) {
            return true;
        }
    }
    return false;
}

std::string get_arg_value(const std::vector<std::string> &args, 
                          const std::string &long_name, const std::string &short_name) {
    for (size_t i = 1; i < args.size(); ++i) {
        std::string value;
        
        if (args[i].find(long_name) == 0) {
            value = remove_prefix(args[i], long_name);
        } else if (args[i].find(short_name) == 0) {
            value = remove_prefix(args[i], short_name);
        } else {
            continue;
        }

        if (!value.empty() && value[0] == '=') {
            return remove_prefix(value, "=");
        }
        if (value.empty() && i + 1 < args.size()) {
            return args[i + 1];
        }
    }
    throw std::runtime_error("argument " + long_name + " not specified");
}

Mode parse_mode(const std::vector<std::string> &args) {
    std::string value = get_arg_value(args, "--mode", "-m");
    if (value == "encrypt" || value == "e") {
        return Mode::Encrypt;
    }
    if (value == "decrypt" || value == "d") {
        return Mode::Decrypt;
    }
    throw std::runtime_error("unknown operating mode: " + value);
}

Algorithm parse_algorithm(const std::vector<std::string> &args) {
    std::string value = get_arg_value(args, "--algorithm", "-a");
    if (value == "aes") {
        return Algorithm::Aes;
    }
    if (value == "des") {
        return Algorithm::Des;
    }
    if (value == "rava") {
        return Algorithm::Rava;
    }
    throw std::runtime_error("unknown algorithm: " + value);
}

std::vector<uint8_t> parse_key(const std::vector<std::string> &args, size_t size) {
    std::string value = get_arg_value(args, "--key", "-k");

    if (value == "generate" || value == "gen") {
        return random_vector(size);
    }
    if (value == "console" || value == "terminal") {
        return input_bytes(size);
    }
    if (!std::filesystem::exists(value)) {
        throw std::runtime_error("Файл ключа не существует: " + value);
    }
    return read_n_bytes(value, size);
}

void parse_save_key(const std::vector<std::string> &args, const std::vector<uint8_t> &key) {
    std::string value;
    try {
        value = get_arg_value(args, "--save-key", "-s");
    } catch (...) {
        return;
    }
    if (std::filesystem::exists(value)) {
        throw std::runtime_error("Файл для сохранения ключа уже существует: " + value);
    }
    write_bytes(value, key);
}

std::vector<uint8_t> parse_input(const std::vector<std::string> &args) {
    std::string value = get_arg_value(args, "--input", "-i");
    
    if (value == "console" || value == "terminal") {
        return input_all_bytes();
    }
    if (value.starts_with("0x")) {
        return parse_hex_all(value);
    }
    return read_all_bytes(value);
}

ParsedOutput parse_output(const std::vector<std::string> &args) {
    if (const std::string value = get_arg_value(args, "--output", "-o"); value == "binary" || value == "bin") {
        return ParsedOutput{"", Output::Binary};
    } else {
        if (value == "text") {
            return ParsedOutput{"", Output::Text};
        }
        if (value == "hex") {
            return ParsedOutput{"", Output::Hex};
        }
        if (std::filesystem::exists(value)) {
            throw std::runtime_error("Output file already exists: " + value);
        }
        return ParsedOutput{value, Output::File};
    }
}

bool parse_help(const std::vector<std::string> &args) {
    if (has_arg(args, "--help", "-h") || args.size() <= 1) {
        std::cout << R"(
usage: cryptum [OPTIONS]

Uses encryption algorithms AES, DES and RAVA for data conversion.

Required:
  -a, --algorithm {des, aes, rava}                  The encryption algorithm used
  -m, --mode {encrypt, decrypt}                     Operating mode
  -k, --key {generate, console, /path/to/file}      Key source
  -i, --input {console, /path/to/file}              Input source
  -o, --output {binary, text, hex, /path/to/file}   Output place

Optional:
  -h, --help                                        Show this help and exit
  -s, --save-key /path/to/file                      The key saving file

Examples:
  cryptum -a des -m decrypt -k console -i 0x5b245b08e90603dff5a6f08d0457be95 -o text
  cryptum -a aes -m decrypt -k key.txt -i output.bin -o binary
  cryptum -a rava -m encrypt -k generate -s key.txt -i console -o hex
)" << std::endl;
        return true;
    }
    return false;
}