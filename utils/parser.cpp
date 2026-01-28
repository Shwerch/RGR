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
    throw std::runtime_error("the required argument is not specified: " + long_name);
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
    if (value == "ngea") {
        return Algorithm::Ngea;
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
        throw std::runtime_error("the algorithm key file does not exist: " + value);
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
        throw std::runtime_error("the file for saving the algorithm key already exists: " + value);
    }
    write_bytes(value, key);
}

std::vector<uint8_t> parse_input(const std::vector<std::string> &args) {
    std::string value = get_arg_value(args, "--input", "-i");
    
    if (value == "console" || value == "terminal") {
        return input_all_bytes();
    }
    if (value.starts_with("0x")) {
        return parse_hex_to_vec(value);
    }
    return read_all_bytes(value);
}

ParsedOutput parse_output(const std::vector<std::string> &args) {
    ParsedOutput parsed_output;
    std::optional<Output> output_mode;

    std::string output = get_arg_value(args, "--output", "-o");
    if (output == "binary" || output == "bin") {
        output_mode = Output::Binary;
    }
    else if (output == "text") {
        output_mode =  Output::Text;
    }
    else if (output == "hex") {
        output_mode =  Output::Hex;
    } else {
        if (std::filesystem::exists(output)) {
            throw std::runtime_error("the output data file already exists: " + output);
        }
        output_mode = Output::File;
        parsed_output.path = output;
    }

    if (output_mode == std::nullopt)
        throw std::runtime_error("output destination not specified");
    parsed_output.mode = output_mode.value();
    return parsed_output;
}

bool parse_help(const std::vector<std::string> &args) {
    if (has_arg(args, "--help", "-h") || args.size() <= 1) {
        std::cout << HELP << std::endl;
        return true;
    }
    return false;
}