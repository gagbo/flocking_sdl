add_library(${PROJECT_NAME}_food food.cpp)
target_link_libraries(${PROJECT_NAME}_food ${PROJECT_NAME}_entity)
target_include_directories(${PROJECT_NAME}_food PUBLIC ${EIGEN3_INCLUDE_DIR})

install(TARGETS ${PROJECT_NAME}_food DESTINATION lib)
install(FILES food.h DESTINATION include/food)
