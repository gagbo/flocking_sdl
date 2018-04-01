add_library(${PROJECT_NAME}_mainwindow mainwindow.cpp)

target_link_libraries(${PROJECT_NAME}_mainwindow SDL2 SDL2_image)
target_link_libraries(${PROJECT_NAME}_mainwindow ${PROJECT_NAME}_world)

install(TARGETS ${PROJECT_NAME}_mainwindow DESTINATION lib)
install(FILES mainwindow.h DESTINATION include/ui/window)
