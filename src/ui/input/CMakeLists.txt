add_library(input user_input.cpp)

target_link_libraries(input SDL2)
install(TARGETS input DESTINATION lib)
install(FILES user_input.h DESTINATION include)
