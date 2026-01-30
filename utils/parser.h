#pragma once

#include <string>
#include <optional>

enum class Algorithm {
    DES,
    AES,
    NGEA
};

enum class Mode {
    Encrypt,
    Decrypt
};

struct Arguments {
    Algorithm algorithm;
    Mode mode;
    std::string inputPath;
    std::string outputPath;
    bool textInput = false;
    bool textOutput = false;
    std::string keyPath;
    bool generateKey = false;
    bool readKey = false;
    std::string saveKeyPath;
    bool writeKey = false;
};

Arguments parser(int argc, char** argv);
