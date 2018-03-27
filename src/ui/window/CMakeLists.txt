add_library(mainwindow mainwindow.cpp)

target_link_libraries(mainwindow SDL2 SDL2_image)
target_link_libraries(mainwindow world)

target_include_directories(mainwindow PUBLIC ${PROJECT_SOURCE_DIR})

install(TARGETS mainwindow DESTINATION lib)
install(FILES mainwindow.h DESTINATION include)
