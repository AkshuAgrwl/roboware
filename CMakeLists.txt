cmake_minimum_required(VERSION 3.14)
project(roboware_core_messages LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library target
add_library(rw_core_messages
    src/core/messages/parser.cpp
)

target_include_directories(rw_core_messages
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Test setup
enable_testing()
add_subdirectory(tests)
