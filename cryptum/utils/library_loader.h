#pragma once
#include <string>
#include "cipher_interface.h"

CipherInterface load_cipher(const std::string &name);

#include "exception.h"
#include <sstream>

inline void MyException::compose_full_message() {
    std::ostringstream oss;
    oss << "Произошла ошибка в " << file_ << " в строке " << line_
        << " в функции " << func_ << '\n'
        << std::runtime_error::what();
    full_msg_ = oss.str();
}

inline MyException::MyException(const std::string &msg, const char *file, const int line, const char *func)
    : std::runtime_error(msg), file_(file ? file : ""), line_(line), func_(func ? func : "") {
    compose_full_message();
}
