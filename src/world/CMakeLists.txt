add_library(${PROJECT_NAME}_world world.cpp)

target_link_libraries(${PROJECT_NAME}_world ${PROJECT_NAME}_mainwindow)
target_link_libraries(${PROJECT_NAME}_world ${PROJECT_NAME}_input)
target_link_libraries(${PROJECT_NAME}_world ${PROJECT_NAME}_entity)

target_include_directories(${PROJECT_NAME}_world PUBLIC ${EIGEN3_INCLUDE_DIR})

install(TARGETS ${PROJECT_NAME}_world DESTINATION lib)
install(FILES world.h kdtree.h DESTINATION include/world)
