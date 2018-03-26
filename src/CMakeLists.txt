project (Flocking_SDL-main)

set (Flocking_VERSION_MAJOR 0)
set (Flocking_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    "${PROJECT_SOURCE_DIR}/FlockingConfig.h.in"
    "${PROJECT_BINARY_DIR}/FlockingConfig.h"
    )

# add the binary tree to the search path for include files
# so that we will find FlockingConfig.h
include_directories ("${PROJECT_BINARY_DIR}")

add_subdirectory(window)

add_executable (flocks main.cpp)
# Maybe linking SDL2 is not necessary since it should be ui
target_link_libraries (flocks SDL2)
target_link_libraries (flocks mainwindow)

install (TARGETS flocks DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/FlockingConfig.h"
             DESTINATION include)
