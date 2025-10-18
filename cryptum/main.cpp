#include <iostream>

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

typedef void (*PrintFuncType)();

int main() {
    const auto lib1_name =
#ifdef _WIN32
        "libaes.dll";
#else
            "libaes.so";
#endif

    const LIB_HANDLE lib1_handle = load_lib(lib1_name);
    if (!lib1_handle) {
        std::cerr << "Error loading library " << lib1_name << std::endl;
        return 1;
    }

    const auto print_func = reinterpret_cast<PrintFuncType>(get_func(lib1_handle, "aes"));
    if (!print_func) {
        std::cerr << "Error finding function aes" << std::endl;
        close_lib(lib1_handle);
        return 1;
    }

    print_func();

    close_lib(lib1_handle);
    return 0;
}