#include "utils/filesystem.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>

std::vector<uint8_t> read_file(const std::string &path) {
	std::ifstream file(path, std::ios::binary);
	if (!file) {
		throw std::runtime_error("Cannot open input file: " + path);
	}
	return std::vector<uint8_t>((std::istreambuf_iterator<char>(file)),
								std::istreambuf_iterator<char>());
}

void write_file(const std::string &path, const std::vector<uint8_t> &data) {
	std::ofstream file(path, std::ios::binary);
	if (!file) {
		throw std::runtime_error("Cannot open output file: " + path);
	}
	file.write(reinterpret_cast<const char *>(data.data()), data.size());
}

std::vector<uint8_t> read_from_stdin() {
	std::cin.sync_with_stdio(false);
	std::cin >> std::noskipws;
	return std::vector<uint8_t>((std::istream_iterator<char>(std::cin)),
								std::istream_iterator<char>());
}

void write_to_stdout(const std::vector<uint8_t> &data) {
	std::cout.write(reinterpret_cast<const char *>(data.data()), data.size());
	std::cout.flush();
}
