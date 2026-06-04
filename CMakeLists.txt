cmake_minimum_required(VERSION 3.20)

project(agentbox
    VERSION 0.1.0
    LANGUAGES CXX
)

add_executable(agentbox
    src/main.cpp
    src/init.cpp
    src/cli/cli_init.cpp
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

target_compile_features(agentbox PRIVATE cxx_std_20)

target_include_directories(agentbox
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cli
        ${CMAKE_CURRENT_SOURCE_DIR}/external
)

target_compile_options(agentbox
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)