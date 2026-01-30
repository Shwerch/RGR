#include "parser.h"
#include "io_handlers.h"
#include "library_loader.h"

#include <iostream>

#ifdef _WIN32
    #include <fcntl.h>
    #include <io.h>
#endif

int main(int argc, char** argv) {
    #ifdef _WIN32
        _setmode(_fileno(stdin), _O_BINARY);
        _setmode(_fileno(stdout), _O_BINARY);
    #endif
    try {
        Arguments args = parser(argc, argv);
        std::vector<uint8_t> key = getKey(args);
        saveKey(args, key);
        std::vector<uint8_t> inputData = getInputData(args);

        Library* lib = new Library{
            args.algorithm == Algorithm::DES ? "des" :
            args.algorithm == Algorithm::AES ? "aes" :
            "ngea"
        };
        Function func = lib->get_function(args.mode == Mode::Encrypt ? "encrypt" : "decrypt");

        uint8_t* result_buffer = nullptr;
        size_t result_size = 0;
        Deleter result_deleter  = nullptr;
        bool result = func(
            inputData.data(), 
            inputData.size(), 
            key.data(), 
            &result_buffer,
            &result_size,
            &result_deleter
        );

        if (!result) {
            throw std::runtime_error("Undefined error during encryption algorithm work");
        }

        std::vector<uint8_t> resultData(result_buffer, result_buffer + result_size);
        result_deleter(result_buffer);
        delete lib;
        writeOutputData(args, resultData);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
