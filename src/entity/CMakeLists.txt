add_library(entity entity.cpp)

add_subdirectory(ant)
add_subdirectory(food)

target_link_libraries(entity world)
target_link_libraries(entity ant food)

target_include_directories(entity PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(entity PUBLIC ${EIGEN3_INCLUDE_DIR})

install(TARGETS entity DESTINATION lib)
install(FILES entity.h DESTINATION include)
