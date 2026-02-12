#include "utils/io_handlers.hpp"
#include "utils/filesystem.hpp"

#include <cstdint>
#include <iostream>
#include <optional>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

std::vector<uint8_t> generate_random_key(size_t length) {
	std::vector<uint8_t> key(length);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 255);
	for (auto &byte : key) {
		byte = static_cast<uint8_t>(dis(gen));
	}
	return key;
}

size_t get_key_size(Algorithm algo) {
	switch (algo) {
	case Algorithm::DES:
		return 8;
	case Algorithm::AES:
		return 32;
	case Algorithm::NGEA:
		return 32;
	default:
		return 32;
	}
}

std::vector<uint8_t> get_key(const Arguments &args) {
	if (!args.keyPath.empty()) {
		return read_file(args.keyPath);
	}

	if (args.readKey) {
		return read_from_stdin();
	}

	if (args.generateKey) {
		return generate_random_key(get_key_size(args.algorithm));
	}

	throw std::runtime_error("No key source specified");
}

void save_key(const Arguments &args, const std::vector<uint8_t> &key) {
	if (!args.saveKeyPath.empty()) {
		write_file(args.saveKeyPath, key);
	} else if (args.writeKey) {
		write_to_stdout(key);
	}
}

std::vector<uint8_t> get_input_data(const Arguments &args) {
	if (args.manualInput) {
		return read_string_as_bytes();
	}
	if (!args.inputPath.empty()) {
		return read_file(args.inputPath);
	}
	return read_from_stdin();
}

void write_output_data(const Arguments &args, const std::vector<uint8_t> &data) {
	if (!args.outputPath.empty()) {
		write_file(args.outputPath, data);
	} else {
		write_to_stdout(data);
	}
}

std::vector<uint8_t> read_string_as_bytes() {
	std::cout << "Enter input string: ";
	std::string input;
	std::getline(std::cin, input);
	std::string view(input);
	std::vector<uint8_t> bytes;
	bytes.reserve(view.size());
	for (char c : view) {
		bytes.push_back(static_cast<uint8_t>(c));
	}
	return bytes;
}