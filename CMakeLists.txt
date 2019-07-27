cmake_minimum_required(VERSION 3.14)
project(chip8)

set(CMAKE_CXX_STANDARD 14)

set(SFML_INCLUDE_DIR "C:\\Program Files (x86)\\SFML\\include")
set(SFML_LIBRARY_DIR "C:\\Program Files (x86)\\SFML\\lib")
set(SFML_DIR "C:\\Program Files (x86)\\SFML\\lib\\cmake\\SFML")

link_directories(SFML_LIBRARY_DIR)
include_directories(SFML_INCLUDE_DIR)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)

if(SFML_FOUND)
    message(STATUS "SFML_INCLUDE_DIR: ${SFML_INCLUDE_DIR}")
    message(STATUS "SFML_LIBRARIES: ${SFML_LIBRARIES}")
    message(STATUS "SFML_VERSION: ${SFML_VERSION}")
endif()


#find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)


add_executable(chip8 main.cpp Chip8.cpp Chip8.hpp)
target_link_libraries (chip8 sfml-graphics sfml-window sfml-system)
#target_link_libraries(chip8 PRIVATE sfml-graphics sfml-audio)