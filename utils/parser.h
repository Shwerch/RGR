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
    Ngea,
};

enum class Output {
    Binary,
    Text,
    Hex,
    File
};

struct ParsedOutput {
    std::optional<std::string> path;
    Output mode = Output::Hex;
};

constexpr auto HELP = R"(
usage: cryptum [OPTIONS]

Uses encryption algorithms AES, DES and NGEA for data encryption and decryption.

Required:
  -a, --algorithm {des, aes, ngea}                     The encryption algorithm used
  -m, --mode {encrypt|e, decrypt|d}                    Operating mode
  -k, --key {generate|gen, console, /path/to/file}     Key source
  -i, --input {console, 0x<hex_data>, /path/to/file}   Input source
  -o, --output {binary|bin, text, hex, /path/to/file}  Output place

Optional:
  -h, --help                                           Show this help and exit
  -s, --save-key /path/to/file                         Save the generated key to file

Examples:
  cryptum -a des -m decrypt -k console -i 0x5b245b08e90603dff5a6f08d0457be95 -o text
  cryptum -a aes -m d -k key.bin -i output.bin -o bin
  cryptum -a ngea -m encrypt -k gen -s ngea.key -i console -o hex
  cryptum -a aes -m e -k gen -s aes.key -i console -o ciphertext.bin
)";

Algorithm parse_algorithm(const std::vector<std::string> &args);
Mode parse_mode(const std::vector<std::string> &args);
std::vector<uint8_t> parse_key(const std::vector<std::string> &args, size_t size);
void parse_save_key(const std::vector<std::string> &args, const std::vector<uint8_t> &key);
std::vector<uint8_t> parse_input(const std::vector<std::string> &args);
ParsedOutput parse_output(const std::vector<std::string> &args);
bool parse_help(const std::vector<std::string> &args);