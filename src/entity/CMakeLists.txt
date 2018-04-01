add_library(${PROJECT_NAME}_entity entity.cpp)

add_subdirectory(ant)
add_subdirectory(food)

target_link_libraries(${PROJECT_NAME}_entity ${PROJECT_NAME}_ant ${PROJECT_NAME}_food)

target_include_directories(${PROJECT_NAME}_entity PUBLIC ${EIGEN3_INCLUDE_DIR})

install(TARGETS ${PROJECT_NAME}_entity DESTINATION lib)
install(FILES entity.h DESTINATION include/entity)
