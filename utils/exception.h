#pragma once
#include <stdexcept>
#include <string>

#define EXCEPTION(msg) MyException((msg), __func__)

class MyException final : public std::runtime_error {
    std::string func_;
    std::string full_msg_;
public:
    MyException(const std::string &msg, const char *func);
    const char *what() const noexcept override { return full_msg_.c_str(); }
};