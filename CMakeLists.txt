cmake_minimum_required(VERSION 3.28)
project(Cryptum)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(include)

set(UTILS_SOURCES
        utils/filesystem.cpp
        utils/io_handlers.cpp
        utils/rand_utils.cpp
        utils/parser.cpp
)

add_library(utils STATIC ${UTILS_SOURCES})

if(UNIX AND NOT APPLE)
    target_link_libraries(utils dl)
endif()

add_library(aes SHARED ciphers/aes/aes.cpp)
target_link_libraries(aes utils)

add_library(des SHARED ciphers/des/des.cpp)
target_link_libraries(des utils)

add_library(ngea SHARED ciphers/ngea/ngea.cpp)
target_link_libraries(ngea utils)

add_executable(cryptum cryptum/main.cpp)
target_link_libraries(cryptum utils)
include_directories(utils)

add_custom_command(TARGET cryptum POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:aes>
        $<TARGET_FILE:des>
        $<TARGET_FILE:ngea>
        $<TARGET_FILE_DIR:cryptum>
)
set(ALL_TARGETS utils aes des ngea cryptum)
foreach(TARGET ${ALL_TARGETS})
    if(MSVC)
        target_compile_options(${TARGET} PUBLIC
                $<$<CONFIG:Release>:/O2>
                /utf-8
                /W4
                /WX
        )
    else()
        target_compile_options(${TARGET} PUBLIC
                -std=c++23
                $<$<CONFIG:Release>:-O3>
                -pedantic
                -Wall
                -Wextra
                -Werror
        )
    endif()
endforeach()