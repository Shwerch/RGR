#include "parser.h"
#include "io_handlers.h"

#include <iostream>

int main(int argc, char** argv) {
    try {
        Arguments args = parser(argc, argv);

        std::vector<uint8_t> key = getKey(args);
        saveKey(args, key);

        std::vector<uint8_t> inputData = getInputData(args);
        
        // ...

        std::vector<uint8_t> result = inputData;

        writeOutputData(args, result);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
