cmake_minimum_required(VERSION 3.14)
project(chip8)

set(CMAKE_CXX_STANDARD 14)

add_executable(chip8 main.cpp Disassembler.cpp Disassembler.hpp)