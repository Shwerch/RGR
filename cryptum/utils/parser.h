#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include "cipher_interface.h"

enum class Mode {
    Encrypt,
    Decrypt
};

enum class Output {
    Binary,
    Text,
    Hex,
    File
};

struct ParsedOutput {
    std::string path;
    Output mode;
};

CipherInterface parse_algorithm(const std::vector<std::string> &args);
Mode parse_mode(const std::vector<std::string> &args);
std::vector<uint8_t> parse_key(const std::vector<std::string> &args, size_t size);
void parse_save_key(const std::vector<std::string> &args, const std::vector<uint8_t> &key);
std::vector<uint8_t> parse_input(const std::vector<std::string> &args);
ParsedOutput parse_output(const std::vector<std::string> &args);
bool parse_help(const std::vector<std::string> &args);