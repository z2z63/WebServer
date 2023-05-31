cmake_minimum_required(VERSION 3.26)
project(WebServer)

set(CMAKE_CXX_STANDARD 20)

add_executable(WebServer main.cpp WebServer.cpp WebServer.h HttpResponse.cpp HttpResponse.h HttpRequest.cpp HttpRequest.h ThreadPool.h)
