add_executable(test_flocks main_tests.cpp test_world.cpp)

target_link_libraries(test_flocks SDL2 SDL2_image)
target_link_libraries(test_flocks world)

target_include_directories(test_flocks PUBLIC ${PROJECT_SOURCE_DIR}/src)

install(TARGETS test_flocks DESTINATION bin)

add_test(NAME catch2_test COMMAND test_flocks -s)
