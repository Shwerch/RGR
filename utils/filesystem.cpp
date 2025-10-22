#include "filesystem.h"
#include <fstream>

std::vector<uint8_t> read_n_bytes(const std::string& path, size_t N) {
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("Cannot open file: " + path);
    std::vector<uint8_t> buffer;
    buffer.resize(N);
    in.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(N));
    if (const auto read_count = static_cast<std::size_t>(in.gcount()); read_count < N) {
        buffer.resize(read_count);
    }
    return buffer;
}
std::vector<uint8_t> read_all_bytes(const std::string& path) {
    std::ifstream in(path, std::ios::binary | std::ios::ate);
    if (!in) throw std::runtime_error("Cannot open file: " + path);
    std::streamsize size = in.tellg();
    in.seekg(0, std::ios::beg);
    std::vector<uint8_t> buf(static_cast<std::size_t>(size));
    if (size > 0) {
        in.read(reinterpret_cast<char*>(buf.data()), size);
        if (!in) throw std::runtime_error("Read failed: " + path);
    }
    return buf;
}
void write_bytes(const std::string &path, const std::vector<uint8_t> &data) {
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    if (!out) {
        throw std::runtime_error("Cannot open file for writing: " + path);
    }
    if (!data.empty()) {
        out.write(reinterpret_cast<const char*>(data.data()),
                  static_cast<std::streamsize>(data.size()));
        if (!out) {
            throw std::runtime_error("Write failed: " + path);
        }
    }
}
