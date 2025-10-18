#pragma once

#include <stdexcept>
#include <string>

#define EXCEPTION(msg) MyException((msg), __FILE__, __LINE__, __func__)

class MyException final : public std::runtime_error {
    std::string file_;
    int line_;
    std::string func_;
    std::string full_msg_;
    void compose_full_message();
public:
    MyException(const std::string &msg, const char *file, int line, const char *func);
    const char *what() const noexcept override { return full_msg_.c_str(); }
};