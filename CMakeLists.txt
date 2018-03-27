cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 14)
project(Flocking_SDL)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED QUIET)
find_package(SDL2_image REQUIRED QUIET)
find_package(Eigen3 REQUIRED QUIET)

enable_testing()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")

set(BUILD_SHARED_LIBS OFF)

set(CMAKE_CXX_FLAGS "-O2 -pipe \
-Wall -Wextra -D_FORTIFY_SOURCE=2 -fexceptions")
set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=address -fno-omit-frame-pointer \
-fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_LINKER_FLAGS_DEBUG "-lasan")

add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(test)
