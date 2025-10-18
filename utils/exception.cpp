#include "exception.h"
#include <sstream>

MyException::MyException(const std::string &msg, const char *func)
    : std::runtime_error(msg), func_(func ? func : "") {
    std::ostringstream oss;
    oss << "Error happened in function " << func_ << ": " << std::endl << std::runtime_error::what();
    full_msg_ = oss.str();
}
