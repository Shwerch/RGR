#include "utils/io_handlers.hpp"
#include "utils/filesystem.hpp"

#include <random>
#include <stdexcept>

std::vector<uint8_t> generateRandomKey(size_t length) {
	std::vector<uint8_t> key(length);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 255);
	for (auto &byte : key) {
		byte = static_cast<uint8_t>(dis(gen));
	}
	return key;
}

size_t getKeySize(Algorithm algo) {
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

std::vector<uint8_t> getKey(const Arguments &args) {
	if (!args.keyPath.empty()) {
		return readFile(args.keyPath);
	}

	if (args.readKey) {
		return readFromStdin();
	}

	if (args.generateKey) {
		return generateRandomKey(getKeySize(args.algorithm));
	}

	throw std::runtime_error("No key source specified");
}

void saveKey(const Arguments &args, const std::vector<uint8_t> &key) {
	if (!args.saveKeyPath.empty()) {
		writeFile(args.saveKeyPath, key);
	} else if (args.writeKey) {
		writeToStdout(key);
	}
}

std::vector<uint8_t> getInputData(const Arguments &args) {
	if (!args.inputPath.empty()) {
		return readFile(args.inputPath);
	}
	return readFromStdin();
}

void writeOutputData(const Arguments &args, const std::vector<uint8_t> &data) {
	if (!args.outputPath.empty()) {
		writeFile(args.outputPath, data);
	} else {
		writeToStdout(data);
	}
}
