cmake_minimum_required(VERSION 3.14)
project(chip8)

set(CMAKE_CXX_STANDARD 14)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(chip8 main.cpp Chip8.cpp Chip8.hpp)

target_link_libraries(chip8 PRIVATE sfml-graphics sfml-audio)