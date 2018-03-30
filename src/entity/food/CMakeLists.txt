add_library(food food.cpp)
target_link_libraries(food entity)
target_include_directories(food PUBLIC ${EIGEN3_INCLUDE_DIR})

install(TARGETS food DESTINATION lib)
install(FILES food.h DESTINATION include)
