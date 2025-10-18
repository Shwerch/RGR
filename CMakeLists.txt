cmake_minimum_required(VERSION 4.0)
project(Cryptum)

set(CMAKE_CXX_STANDARD 20)

add_executable(Cryptum cryptum/main.cpp)

add_library(aes SHARED aes/aes.cpp)
add_library(des SHARED des/des.cpp)

target_link_libraries(Cryptum aes des)