add_library(${PROJECT_NAME}_ant ant.cpp)
target_link_libraries(${PROJECT_NAME}_ant ${PROJECT_NAME}_entity)
target_link_libraries(${PROJECT_NAME}_ant ${PROJECT_NAME}_json)
target_include_directories(${PROJECT_NAME}_ant PUBLIC ${EIGEN3_INCLUDE_DIR})

install(TARGETS ${PROJECT_NAME}_ant DESTINATION lib)
install(FILES ant.h DESTINATION include/ant)
