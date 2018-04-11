set (Flocking_VERSION_MAJOR 0)
set (Flocking_VERSION_MINOR 2)
set (Install_data_dir "\"${CMAKE_INSTALL_PREFIX}/data/\"")

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    "${PROJECT_SOURCE_DIR}/src/FlockingConfig.h.in"
    "${PROJECT_SOURCE_DIR}/src/FlockingConfig.h"
    )

# add the binary tree to the search path for include files
# so that we will find FlockingConfig.h
include_directories ("${PROJECT_BINARY_DIR}/src")

add_subdirectory(ui)
add_subdirectory(entity)
add_subdirectory(world)
add_subdirectory(jsoncpp)

add_executable (flocks main.cpp)
# Maybe linking SDL2 is not necessary since it should be ui
target_link_libraries (flocks SDL2)
target_link_libraries (flocks ${PROJECT_NAME}_mainwindow ${PROJECT_NAME}_input ${PROJECT_NAME}_entity ${PROJECT_NAME}_world)
target_link_libraries (flocks ${PROJECT_NAME}_json)

install (TARGETS flocks DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/src/FlockingConfig.h"
             DESTINATION include)
