cmake_minimum_required(VERSION 3.28)
project(Cryptum)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(MSVC)
    add_compile_options(/utf-8)
else()
endif()

include_directories(include)

set(UTILS_SOURCES
        utils/filesystem.cpp
        utils/hex_utils.cpp
        utils/io_utils.cpp
        utils/library_loader.cpp
        utils/parser.cpp
        utils/rand_utils.cpp
        utils/string_utils.cpp
        utils/pkcs7.cpp
)

add_library(utils STATIC ${UTILS_SOURCES})

if(UNIX AND NOT APPLE)
    target_link_libraries(utils dl)
endif()

add_library(aes SHARED aes/aes.cpp)
target_link_libraries(aes utils)

add_library(des SHARED des/des.cpp)
target_link_libraries(des utils)

add_library(rava SHARED rava/rava.cpp)
target_link_libraries(rava utils)

add_executable(cryptum cryptum/main.cpp)
target_link_libraries(cryptum utils)

add_custom_command(TARGET cryptum POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:aes>
        $<TARGET_FILE:des>
        $<TARGET_FILE:rava>
        $<TARGET_FILE_DIR:cryptum>
)