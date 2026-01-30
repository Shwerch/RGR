#include "parser.h"

#include <string>
#include <stdexcept>
#include <filesystem>
#include <optional>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <random>
#include <cstdlib>

std::vector<uint8_t> hex_to_bytes(const std::string& hex) {
    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        char byte = (char)strtol(byteString.c_str(), NULL, 16);
        bytes.push_back(byte);
    }
    return bytes;
}

std::string bytes_to_hex(const std::vector<uint8_t>& bytes) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int b : bytes) {
        ss << std::setw(2) << (int)b;
    }
    return ss.str();
}

std::vector<uint8_t> generate_random_bytes(size_t length) {
    std::vector<uint8_t> bytes(length);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    for (size_t i = 0; i < length; ++i) {
        bytes[i] = static_cast<uint8_t>(dis(gen));
    }
    return bytes;
}

std::vector<uint8_t> read_stream_binary(std::istream& input) {
    return std::vector<uint8_t>(std::istreambuf_iterator<char>(input), {});
}

std::string read_stream_string(std::istream& input) {
    return std::string(std::istreambuf_iterator<char>(input), {});
}

void print_help() {
    std::cout << HELP_TEXT << std::endl;
    exit(0);
}

RawArguments parse_arguments(int argc, const char** argv) {
    if (argc < 2) {
        print_help();
    }

    RawArguments args;
    bool output_format_set = false;

    for (int i = 1; i < argc; ++i) {
        std::string current_arg = argv[i];
        std::string key;
        std::string val;
        bool has_val = false;

        if (current_arg.length() > 2 && current_arg.substr(0, 2) == "--") {
            size_t eq = current_arg.find('=');
            if (eq != std::string::npos) {
                key = current_arg.substr(0, eq);
                val = current_arg.substr(eq + 1);
                has_val = true;
            } else {
                key = current_arg;
            }
        } else {
            key = current_arg;
        }

        if (key == "-a" || key == "--algorithm") {
            if (!has_val && i + 1 < argc && argv[i + 1][0] != '-') {
                val = argv[++i];
                has_val = true;
            }
            if (has_val) {
                if (val == "des") args.algorithm = Algorithm::des;
                else if (val == "aes") args.algorithm = Algorithm::aes;
                else if (val == "ngea") args.algorithm = Algorithm::ngea;
            }
        } else if (key == "-m" || key == "--mode") {
            if (!has_val && i + 1 < argc && argv[i + 1][0] != '-') {
                val = argv[++i];
                has_val = true;
            }
            if (has_val) {
                if (val == "e" || val == "encrypt") args.mode = Mode::encrypt;
                else if (val == "d" || val == "decrypt") args.mode = Mode::decrypt;
            }
        } else if (key == "-i" || key == "--input") {
            if (!has_val && i + 1 < argc && argv[i + 1][0] != '-') {
                val = argv[++i];
                has_val = true;
            }
            if (has_val) args.input = val;
        } else if (key == "-o" || key == "--output") {
            if (!has_val && i + 1 < argc && argv[i + 1][0] != '-') {
                val = argv[++i];
                has_val = true;
            }
            if (has_val) args.output = val;
        } else if (key == "-I" || key == "--in-format") {
            if (!has_val && i + 1 < argc && argv[i + 1][0] != '-') {
                val = argv[++i];
                has_val = true;
            }
            if (has_val) {
                if (val == "text") args.input_format = Input::text;
                else if (val == "hex") args.input_format = Input::hex;
                else if (val == "binary") args.input_format = Input::binary;
            }
        } else if (key == "-O" || key == "--out-format") {
            if (!has_val && i + 1 < argc && argv[i + 1][0] != '-') {
                val = argv[++i];
                has_val = true;
            }
            if (has_val) {
                if (val == "text") args.output_format = Output::text;
                else if (val == "hex") args.output_format = Output::hex;
                else if (val == "binary") args.output_format = Output::binary;
                output_format_set = true;
            }
        } else if (key == "-k" || key == "--key") {
            if (!has_val && i + 1 < argc && argv[i + 1][0] != '-') {
                val = argv[++i];
                has_val = true;
            }
            if (has_val) args.key = val;
        } else if (key == "-g" || key == "--generate-key") {
            args.generate_key = true;
        } else if (key == "-r" || key == "--read-key") {
            args.read_key = true;
        } else if (key == "-p" || key == "--promt-key") {
            args.promt_key = true;
        } else if (key == "-R" || key == "--read-key-format") {
            if (!has_val && i + 1 < argc && argv[i + 1][0] != '-') {
                val = argv[++i];
                has_val = true;
            }
            if (has_val) {
                if (val == "hex") args.read_key_format = ReadFormat::hex;
                else if (val == "binary") args.read_key_format = ReadFormat::binary;
            }
        } else if (key == "-S" || key == "--save-key") {
            if (!has_val && i + 1 < argc && argv[i + 1][0] != '-') {
                val = argv[++i];
                has_val = true;
            }
            if (has_val) args.save_key = val;
        } else if (key == "-w" || key == "--write-key") {
            args.write_key = true;
        } else if (key == "-W" || key == "--write-key-format") {
            if (!has_val && i + 1 < argc && argv[i + 1][0] != '-') {
                val = argv[++i];
                has_val = true;
            }
            if (has_val) {
                if (val == "hex") args.write_key_format = WriteFormat::hex;
                else if (val == "binary") args.write_key_format = WriteFormat::binary;
            }
        } else if (key == "-h" || key == "--help") {
            args.help = true;
        }
    }

    if (!output_format_set) {
        if (args.mode == Mode::decrypt) {
            args.output_format = Output::text;
        } else {
            args.output_format = Output::binary;
        }
    }

    return args;
}

Arguments check_arguments(const RawArguments& args) {

    if (args.help.has_value() && args.help.value()) {
        print_help();
    }

    if (!args.algorithm.has_value()) {
        throw std::runtime_error("Invalid arguments: --algorothm is not provided.");
    }

    if (!args.mode.has_value()) {
        throw std::runtime_error("Invalid arguments: --mode is not provided.");
    }

    if (!args.input.has_value() && args.read_key) {
        throw std::runtime_error("Invalid arguments: --input is not provided, but --read-key is specified. Key cannot be read from stdin if input is also stdin.");
    }

    if (!args.output.has_value() && args.write_key) {
        throw std::runtime_error("Invalid arguments: --output is not provided, but --write-key is specified. Key cannot be written to stdout if output is also stdout.");
    }

    int key_input_count = 0;
    if (args.key.has_value()) ++key_input_count;
    if (args.generate_key) ++key_input_count;
    if (args.read_key) ++key_input_count;
    if (args.promt_key) ++key_input_count;

    if (key_input_count == 0) {
        throw std::runtime_error("Invalid arguments: No key source specified. Please provide one of: --key, --generate-key, --read-key, --promt-key.");
    }

    if (key_input_count > 1) {
        throw std::runtime_error("Invalid arguments: Multiple key sources specified. Please specify only one of: --key, --generate-key, --read-key, --promt-key.");
    }

    int key_output_count = 0;
    if (args.save_key.has_value()) ++key_output_count;
    if (args.write_key) ++key_output_count;

    if (key_output_count > 1) {
        throw std::runtime_error("Invalid arguments: Multiple key outputs specified. Please specify only one of: --save-key, --write-key.");
    }

    Arguments result;
    result.algorithm = args.algorithm.value();
    result.mode = args.mode.value();
    result.input_format = args.input_format.value();
    result.output_format = args.output_format.value();
    if (args.input.has_value()) {
        result.input = args.input.value();
    }
    if (args.output.has_value()) {
        result.output = args.output.value();
    }
    if (args.key.has_value()) {
        result.key = args.key.value();
    }
    result.generate_key = args.generate_key.value();
    result.read_key = args.read_key.value();
    result.promt_key = args.promt_key.value();
    result.read_key_format = args.read_key_format.value();
    if (args.save_key.has_value()) {
        result.save_key = args.save_key.value();
    }
    result.write_key = args.write_key.value();
    result.write_key_format = args.write_key_format.value();
    result.help = args.help.value();

    return result;
    
}

std::vector<uint8_t> get_key(const Arguments& args) {
    if (args.generate_key) {
        size_t key_len = 16;
        if (args.algorithm == Algorithm::des) key_len = 8;
        else if (args.algorithm == Algorithm::aes) key_len = 32;
        else if (args.algorithm == Algorithm::ngea) key_len = 32;
        return generate_random_bytes(key_len);
    }

    std::string raw_input;
    
    if (args.key.has_value()) {
        std::ifstream file(*args.key, std::ios::binary);
        if (!file.is_open()) throw std::runtime_error("Cannot open key file");
        
        if (args.read_key_format == ReadFormat::binary) {
            return read_stream_binary(file);
        } else {
            raw_input = read_stream_string(file);
        }
    } else if (args.read_key) {
        if (args.read_key_format == ReadFormat::binary) {
            return read_stream_binary(std::cin);
        } else {
            raw_input = read_stream_string(std::cin);
        }
    } else if (args.promt_key) {
        std::cout << "Enter key: ";
        std::getline(std::cin, raw_input);
    }

    if (args.read_key_format == ReadFormat::hex) {
        return hex_to_bytes(raw_input);
    }
    
    return std::vector<uint8_t>(raw_input.begin(), raw_input.end());
}

void export_key(const Arguments& args, const std::vector<uint8_t>& key_data) {
    if (!args.save_key.has_value() && !args.write_key) return;

    std::string output_data;
    bool binary_mode = false;

    if (args.write_key_format == WriteFormat::hex) {
        output_data = bytes_to_hex(key_data);
    } else {
        binary_mode = true;
    }

    if (args.save_key.has_value()) {
        std::ofstream file(*args.save_key, std::ios::binary);
        if (!file.is_open()) throw std::runtime_error("Cannot open save key file");
        
        if (binary_mode) {
            file.write(reinterpret_cast<const char*>(key_data.data()), key_data.size());
        } else {
            file << output_data;
        }
    } else if (args.write_key) {
        if (binary_mode) {
            std::cout.write(reinterpret_cast<const char*>(key_data.data()), key_data.size());
        } else {
            std::cout << output_data;
        }
    }
}

std::vector<uint8_t> get_input(const Arguments& args) {
    std::vector<uint8_t> result;
    
    if (args.input.has_value()) {
        std::ifstream file(*args.input, std::ios::binary);
        if (!file.is_open()) throw std::runtime_error("Cannot open input file");
        
        if (args.input_format == Input::binary) {
            return read_stream_binary(file);
        } else {
            std::string text = read_stream_string(file);
            if (args.input_format == Input::hex) {
                return hex_to_bytes(text);
            } else {
                return std::vector<uint8_t>(text.begin(), text.end());
            }
        }
    } else {
        if (args.input_format == Input::binary) {
            return read_stream_binary(std::cin);
        } else {
            std::string text = read_stream_string(std::cin);
             if (args.input_format == Input::hex) {
                return hex_to_bytes(text);
            } else {
                return std::vector<uint8_t>(text.begin(), text.end());
            }
        }
    }
}