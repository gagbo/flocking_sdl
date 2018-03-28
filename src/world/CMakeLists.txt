add_library(world world.cpp)

target_link_libraries(world mainwindow)
target_link_libraries(world entity ant)

target_include_directories(world PUBLIC ${EIGEN3_INCLUDE_DIR})
target_include_directories(world PUBLIC ${PROJECT_SOURCE_DIR})

install(TARGETS world DESTINATION lib)
install(FILES world.h DESTINATION include)
