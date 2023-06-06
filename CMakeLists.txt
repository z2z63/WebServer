cmake_minimum_required(VERSION 3.26)
project(WebServer)

set(CMAKE_CXX_STANDARD 20)
find_package(YAML-CPP REQUIRED)
find_package(mstch CONFIG REQUIRED)

add_executable(WebServer src/main.cpp src/WebServer.cpp include/WebServer.h src/HttpResponse.cpp include/HttpResponse.h src/HttpRequest.cpp include/HttpRequest.h include/ThreadPool.h)
target_link_libraries(WebServer ${YAML_CPP_LIBRARIES} mstch::mstch)
target_include_directories(WebServer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)