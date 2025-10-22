#include "library_loader.h"

#include <iostream>

DynamicLibrary::DynamicLibrary(const std::string &name, const std::string &func) :
    handle(load_lib(MAKE_LIB_NAME(name).c_str())),
    function(reinterpret_cast<Function>(get_func(handle, func.c_str()))) {
    if (!handle) {
        throw std::runtime_error("Error loading library " + name);
    }
    if (!function) {
        close_lib(handle);
        throw std::runtime_error("Error finding function " + func);
    }
}
DynamicLibrary::~DynamicLibrary() {
    close_lib(handle);
}