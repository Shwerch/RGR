#include "utils/io_handlers.hpp"
#include "utils/library_loader.hpp"
#include "utils/parser.hpp"

#include <iostream>

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

int main(int argc, char **argv) {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	_setmode(_fileno(stdin), _O_BINARY);
	_setmode(_fileno(stdout), _O_BINARY);
#endif
	try {
		Arguments args = parser(argc, argv);
		std::vector<uint8_t> key = getKey(args);
		saveKey(args, key);
		std::vector<uint8_t> inputData = getInputData(args);

		Library *lib = new Library{args.algorithm == Algorithm::DES	  ? "des"
								   : args.algorithm == Algorithm::AES ? "aes"
																	  : "ngea"};
		Function func = lib->get_function(args.mode == Mode::Encrypt ? "encrypt" : "decrypt");

		uint8_t *result_buffer = nullptr;
		size_t result_size = 0;
		Deleter result_deleter = nullptr;
		uint64_t result = func(inputData.data(), inputData.size(), key.data(), &result_buffer,
							   &result_size, &result_deleter);

		switch (result) {
		case 0:
			break;
		case 1:
			throw std::runtime_error(std::string("Invalid input data for ") +
									 (args.mode == Mode::Encrypt ? "encryption" : "decryption") +
									 " algorithm in DLL");
		default:
			throw std::runtime_error(std::string("Unknown error happend in ") +
									 (args.mode == Mode::Encrypt ? "encryption" : "decryption") +
									 " algorithm in DLL");
		}

		std::vector<uint8_t> resultData(result_buffer, result_buffer + result_size);
		result_deleter(result_buffer);
		delete lib;
		writeOutputData(args, resultData);
	} catch (const std::runtime_error &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
