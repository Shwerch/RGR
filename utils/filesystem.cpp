#include "filesystem.h"
#include "exception.h"
#include <fstream>

std::vector<uint8_t> read_bytes(const std::string &path, size_t size) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw EXCEPTION("Не удалось открыть файл: " + path);
    }

    std::vector<uint8_t> buffer(size);
    file.read(reinterpret_cast<char*>(buffer.data()), size);
    
    if (!file) {
        throw EXCEPTION("Не удалось прочитать " + std::to_string(size) + " байт из файла");
    }

    return buffer;
}

std::vector<uint8_t> read_all_bytes(const std::string &path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw EXCEPTION("Не удалось открыть файл: " + path);
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw EXCEPTION("Не удалось прочитать файл: " + path);
    }

    return buffer;
}

void write_bytes(const std::string &path, const std::vector<uint8_t> &data) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw EXCEPTION("Не удалось открыть файл для записи: " + path);
    }

    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    if (!file) {
        throw EXCEPTION("Не удалось записать данные в файл: " + path);
    }
}