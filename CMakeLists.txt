cmake_minimum_required(VERSION 4.0)
project(Cryptum)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
)

set(AES_SOURCES
        #aes/constants.cpp
        #aes/aes_core.cpp
        #aes/key_schedule.cpp
        #aes/ctr_mode.cpp
        aes/aes.cpp
)

#set(DES_SOURCES
#        des/constants.cpp
#        des/des_core.cpp
#        des/ctr_mode.cpp
#        des/des.cpp
#)

add_library(utils STATIC ${UTILS_SOURCES})

if(UNIX AND NOT APPLE)
    target_link_libraries(utils dl)
endif()

add_library(aes SHARED ${AES_SOURCES})
target_link_libraries(aes utils)

#add_library(des SHARED ${DES_SOURCES})
#target_link_libraries(des utils)

#add_library(rava SHARED rava/rava.cpp)
#target_link_libraries(rava utils)

add_executable(Cryptum cryptum/main.cpp)
target_link_libraries(Cryptum utils)

add_custom_command(TARGET Cryptum POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:aes>
        #$<TARGET_FILE:des>
        #$<TARGET_FILE:rava>
        $<TARGET_FILE_DIR:Cryptum>
)