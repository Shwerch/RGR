#include "filesystem.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <iterator>

std::vector<uint8_t> readFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open input file: " + path);
    }
    return std::vector<uint8_t>((std::istreambuf_iterator<char>(file)),
                                 std::istreambuf_iterator<char>());
}

void writeFile(const std::string& path, const std::vector<uint8_t>& data) {
    std::ofstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open output file: " + path);
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

std::vector<uint8_t> readFromStdin() {
    // Note: Reopening cin in binary mode is platform specific, 
    // assuming standard stream reading here for portability or text/binary mix.
    std::cin >> std::noskipws;
    return std::vector<uint8_t>((std::istream_iterator<char>(std::cin)),
                                 std::istream_iterator<char>());
}

void writeToStdout(const std::vector<uint8_t>& data) {
    std::cout.write(reinterpret_cast<const char*>(data.data()), data.size());
}
