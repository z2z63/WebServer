cmake_minimum_required(VERSION 3.26)
project(WebServer)

set(CMAKE_CXX_STANDARD 20)
find_package(YAML-CPP REQUIRED)

add_executable(WebServer main.cpp WebServer.cpp WebServer.h HttpResponse.cpp HttpResponse.h HttpRequest.cpp HttpRequest.h ThreadPool.h)
target_link_libraries(WebServer ${YAML_CPP_LIBRARIES})
include_directories(${YAML_CPP_INCLUDE_DIR})