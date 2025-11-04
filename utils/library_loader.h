#pragma once
#include <cstdint>
#include <string>

#ifdef _WIN32
    #include <windows.h>
    #define LIB_HANDLE HMODULE
    #define load_lib(name) LoadLibraryA(name)
    #define get_func(handle, name) GetProcAddress(handle, name)
    #define close_lib(handle) FreeLibrary(handle)
    #define MAKE_LIB_NAME(name) (name + ".dll")
#else
    #include <dlfcn.h>
    #define LIB_HANDLE void*
    #define load_lib(name) dlopen(name, RTLD_LAZY)
    #define get_func(handle, name) dlsym(handle, name)
    #define close_lib(handle) dlclose(handle)
    #define MAKE_LIB_NAME(name) ("lib" + name + ".so")
#endif

using Function = uint8_t* (*)(const uint8_t* plaintext_ptr, size_t size, const uint8_t* key_ptr, size_t* out_size);

struct Library {
private:
    LIB_HANDLE handle{};
public:
    explicit Library(const std::string &name);
    Function get_function(const std::string &func) const;
    ~Library();
};