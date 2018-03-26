add_library(world world.cpp)
target_include_directories(world PUBLIC "$ENV(EIGEN3_INCLUDE_DIR)")

install(TARGETS world DESTINATION lib)
install(FILES world.h DESTINATION include)
