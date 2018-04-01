add_library(world world.cpp kdtree.cpp)

target_link_libraries(world mainwindow)
target_link_libraries(world entity)

target_include_directories(world PUBLIC ${EIGEN3_INCLUDE_DIR})
target_include_directories(world PUBLIC ${PROJECT_SOURCE_DIR})

install(TARGETS world DESTINATION lib)
install(FILES world.h kdtree.h DESTINATION include/world)
