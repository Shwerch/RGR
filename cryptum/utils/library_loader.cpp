#include "library_loader.h"
#include "exception.h"
#include <filesystem>

#ifdef _WIN32
    #include <windows.h>
    #define LIB_EXTENSION ".dll"
    typedef HMODULE LibHandle;
#else
#include <dlfcn.h>
#define LIB_EXTENSION ".so"
typedef void* LibHandle;
#endif

LibHandle load_library(const std::string &path) {
#ifdef _WIN32
    return LoadLibraryA(path.c_str());
#else
    return dlopen(path.c_str(), RTLD_LAZY);
#endif
}

void* get_function(LibHandle handle, const std::string &name) {
#ifdef _WIN32
    return reinterpret_cast<void*>(GetProcAddress(handle, name.c_str()));
#else
    return dlsym(handle, name.c_str());
#endif
}

CipherInterface load_cipher(const std::string &name) {
    std::string lib_name = "lib" + name + LIB_EXTENSION;

    std::filesystem::path lib_path = std::filesystem::current_path() / lib_name;
    if (!std::filesystem::exists(lib_path)) {
        lib_path = std::filesystem::path(std::filesystem::current_path()).parent_path() / lib_name;
    }

    LibHandle handle = load_library(lib_path.string());
    if (!handle) {
        throw EXCEPTION("Не удалось загрузить библиотеку: " + lib_name);
    }

    auto encrypt_func = reinterpret_cast<std::vector<uint8_t>(*)(
        const std::vector<uint8_t>&, const std::vector<uint8_t>&)>(
        get_function(handle, "encrypt"));
    auto decrypt_func = reinterpret_cast<std::vector<uint8_t>(*)(
        const std::vector<uint8_t>&, const std::vector<uint8_t>&)>(
        get_function(handle, "decrypt"));

    if (!encrypt_func || !decrypt_func) {
        throw EXCEPTION("Не удалось найти функции в библиотеке: " + lib_name);
    }

    size_t key_size = 0;
    if (name == "aes") key_size = 32;
    else if (name == "des") key_size = 8;
    else if (name == "rava") key_size = 32;

    return CipherInterface{encrypt_func, decrypt_func, key_size, name};
}