add_library(${PROJECT_NAME}_input user_input.cpp json_event.cpp)

target_link_libraries(${PROJECT_NAME}_input SDL2)
target_link_libraries(${PROJECT_NAME}_input ${PROJECT_NAME}_json)
install(TARGETS ${PROJECT_NAME}_input DESTINATION lib)
install(FILES user_input.h DESTINATION include/ui/input)
install(FILES json_event.h DESTINATION include/ui/input)
