#include <iostream>

#include "../utils/library_loader.h"
#include "../utils/exception.h"

int main() {
    const auto plaintext = new uint8_t[10];
    const auto key = new uint8_t[32];

    const auto library = DynamicLibrary("aes", "encrypt");
    const auto result = library.function(plaintext, 10, key);

    std::cout << static_cast<unsigned>(plaintext[0]) << std::endl;
    std::cout << static_cast<unsigned>(result[0]) << std::endl;
    return 0;
}