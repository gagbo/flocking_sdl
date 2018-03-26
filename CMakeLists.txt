cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 14)
project(Flocking_SDL)

find_package(SDL2 REQUIRED QUIET)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")

set(BUILD_SHARED_LIBS OFF)

set(CMAKE_CXX_FLAGS "-O2 -pipe \
-Wall -Wextra -D_FORTIFY_SOURCE=2 -fexceptions")
set(CMAKE_CXX_FLAGS_DEBUG "-g \
-fprofile-arcs -ftest-coverage")

add_subdirectory(src)
