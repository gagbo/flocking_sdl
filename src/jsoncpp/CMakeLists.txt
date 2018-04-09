add_library(${PROJECT_NAME}_json jsoncpp.cpp)

install(TARGETS ${PROJECT_NAME}_json DESTINATION lib)
install(FILES json/json.h json/json-forwards.h DESTINATION include/jsoncpp)
