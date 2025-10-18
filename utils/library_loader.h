#pragma once
#include <cstdint>
#include <string>

#ifdef _WIN32
#include <windows.h>
#define LIB_HANDLE HMODULE
#define load_lib(name) LoadLibraryA(name)
#define get_func(handle, name) GetProcAddress(handle, name)
#define close_lib(handle) FreeLibrary(handle)
#else
#include <dlfcn.h>
#define LIB_HANDLE void*
#define load_lib(name) dlopen(name, RTLD_LAZY)
#define get_func(handle, name) dlsym(handle, name)
#define close_lib(handle) dlclose(handle)
#endif

#ifdef _WIN32
    #define MAKE_LIB_NAME(name) (name + ".dll")
#else
    #define MAKE_LIB_NAME(name) ("lib" + name + ".so")
#endif

typedef uint8_t* (*Function)(const uint8_t* text_ptr, size_t size, const uint8_t* key_ptr);

struct DynamicLibrary {
private:
    const LIB_HANDLE handle;
public:
    const Function function;
    DynamicLibrary(const std::string &name, const std::string &func);
    ~DynamicLibrary();
};