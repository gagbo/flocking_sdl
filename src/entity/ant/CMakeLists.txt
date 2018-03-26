add_library(ant ant.cpp)
target_link_libraries(ant entity)
target_include_directories(ant PUBLIC ${EIGEN3_INCLUDE_DIR})

install(TARGETS ant DESTINATION lib)
install(FILES ant.h DESTINATION include)
