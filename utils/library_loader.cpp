#include "library_loader.h"

#include <iostream>

Library::Library(const std::string &name) :
    handle(load_lib(MAKE_LIB_NAME(name).c_str())) {
    if (!handle) {
#ifdef _WIN32
        throw std::runtime_error("Error loading library (Windows): " + name);
#else
        const char* error_msg = dlerror();
        std::string error_details = (error_msg ? error_msg : "unknown error");
        throw std::runtime_error("Error loading library: " + name + ". Details: " + error_details);
#endif
    }
}
Function Library::get_function(const std::string &func) const {
#ifndef _WIN32
    dlerror();
#endif
    const auto function = reinterpret_cast<Function>(get_func(handle, func.c_str()));
    if (!function) {
#ifdef _WIN32
        throw std::runtime_error("Error finding function: " + func);
#else
        const char* error_msg = dlerror();
        std::string error_details = (error_msg ? error_msg : "symbol not found or other error");
        throw std::runtime_error("Error finding function: " + func + ". Details: " + error_details);
#endif
    }
    return function;
}
Library::~Library() {
    close_lib(handle);
}