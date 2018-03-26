add_library(ant ant.cpp)
target_link_libraries(ant mainwindow world)
target_include_directories(ant PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(ant PUBLIC "$ENV(EIGEN3_INCLUDE_DIR)")

install(TARGETS ant DESTINATION lib)
install(FILES ant.h DESTINATION include)
