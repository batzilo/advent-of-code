# Specify the minimum required CMake version.
cmake_minimum_required(VERSION 3.14)

# Set the project information.
project(aoc2022 C)

# Set the C standard to C11.
set(CMAKE_C_STANDARD 11)

# Place the executable in `build/bin`.
# https://stackoverflow.com/a/6595001
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

# Source in the lint definitions (don't lint anything yet).
include(cmake/lint.cmake)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wshadow -Wstrict-overflow -fno-strict-aliasing")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -march=native -std=c99")

# Enable testing for this project.
enable_testing()

add_subdirectory(libs)
