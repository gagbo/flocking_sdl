include(CTest)

add_executable(test_flocks main_tests.cpp test_world.cpp test_kdtree.cpp test_entity.cpp)

target_link_libraries(test_flocks gcov)
target_link_libraries(test_flocks SDL2 SDL2_image)
target_link_libraries(test_flocks ${PROJECT_NAME}_world ${PROJECT_NAME}_entity)
target_link_libraries(test_flocks ${PROJECT_NAME}_json)

target_include_directories(test_flocks PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_include_directories(test_flocks PUBLIC ${EIGEN3_INCLUDE_DIR})

install(TARGETS test_flocks DESTINATION bin)

add_test(NAME catch2_test COMMAND test_flocks -s)
