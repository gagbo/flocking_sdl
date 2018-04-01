add_library(${PROJECT_NAME}_input user_input.cpp)

target_link_libraries(${PROJECT_NAME}_input SDL2)
install(TARGETS ${PROJECT_NAME}_input DESTINATION lib)
install(FILES user_input.h DESTINATION include/ui/input)
