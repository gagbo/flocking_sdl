add_library(mainwindow mainwindow.cpp)
target_link_libraries(mainwindow SDL2 SDL2_image)

install(TARGETS mainwindow DESTINATION lib)
install(FILES mainwindow.h DESTINATION include)
