#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <optional>

enum class Mode {
    Encrypt,
    Decrypt
};

enum class Algorithm {
    Aes,
    Des,
    Rava,
};

enum class Output {
    Binary,
    Text,
    Hex,
    File
};

struct ParsedOutput {
    std::optional<std::string> path = std::nullopt;
    Output mode = Output::Hex;
};

constexpr auto HELP = R"(
usage: cryptum [OPTIONS]

Uses encryption algorithms AES, DES and RAVA for data conversion.

Required:
  -a, --algorithm {des, aes, rava}                  The encryption algorithm used
  -m, --mode {encrypt, decrypt}                     Operating mode
  -k, --key {generate, console, /path/to/file}      Key source
  -i, --input {console, hex data, /path/to/file}    Input source
  -f, --format {binary, text, hex, file}            Output format

Optional:
  -o, --output /path/to/file                        Output place
  -h, --help                                        Show this help and exit
  -s, --save-key /path/to/file                      The key saving file

Examples:
  cryptum -a des -m decrypt -k console -i 0x5b245b08e90603dff5a6f08d0457be95 -o text
  cryptum -a aes -m decrypt -k key.txt -i output.bin -o binary
  cryptum -a rava -m encrypt -k generate -s key.txt -i console -o hex
)";

Algorithm parse_algorithm(const std::vector<std::string> &args);
Mode parse_mode(const std::vector<std::string> &args);
std::vector<uint8_t> parse_key(const std::vector<std::string> &args, size_t size);
void parse_save_key(const std::vector<std::string> &args, const std::vector<uint8_t> &key);
std::vector<uint8_t> parse_input(const std::vector<std::string> &args);
ParsedOutput parse_output(const std::vector<std::string> &args);
bool parse_help(const std::vector<std::string> &args);