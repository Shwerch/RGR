cmake_minimum_required(VERSION 3.28)
project(Cryptum VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(UTILS_SOURCES
    src/utils/filesystem.cpp
    src/utils/io_handlers.cpp
    src/utils/rand_utils.cpp
    src/utils/parser.cpp
    src/utils/library_loader.cpp
)

add_library(utils STATIC ${UTILS_SOURCES})

if(UNIX AND NOT APPLE)
    target_link_libraries(utils dl)
endif()

add_library(aes SHARED src/ciphers/aes/aes.cpp)
target_link_libraries(aes utils)

add_library(des SHARED src/ciphers/des/des.cpp)
target_link_libraries(des utils)

add_library(ngea SHARED src/ciphers/ngea/ngea.cpp)
target_link_libraries(ngea utils)

add_executable(cryptum src/main.cpp)
target_link_libraries(cryptum utils)

add_custom_command(TARGET cryptum POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:aes>
        $<TARGET_FILE:des>
        $<TARGET_FILE:ngea>
        $<TARGET_FILE_DIR:cryptum>
)

set(ALL_TARGETS utils aes des ngea cryptum)

foreach(TARGET ${ALL_TARGETS})
    target_include_directories(${TARGET} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    if(MSVC)
        target_compile_options(${TARGET} PUBLIC
                $<$<CONFIG:Release>:/O2>
                /utf-8
                /W4
                /WX
        )
    else()
        target_compile_options(${TARGET} PUBLIC
                $<$<CONFIG:Release>:-O3>
                -pedantic
                -Wall
                -Wextra
                -Werror
        )
    endif()
endforeach()