#ifndef WEBSERVER_WEBSERVER_H
#define WEBSERVER_WEBSERVER_H


#include <functional>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdexcept>
#include <iostream>
#include <map>
#include <filesystem>
#include <queue>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include <mstch/mstch.hpp>
#include "HttpResponse.h"
#include "HttpRequest.h"
#include "ThreadPool.h"


class WebServer {
private:
    unsigned short port;
    int socketFd;
    std::filesystem::path staticFilePath;
    std::filesystem::path templateFilePath;
    std::map<std::string, std::function<HttpResponse(HttpRequest &)>> routeMap;
    ThreadPool<std::function<void()>> pool;
public:
    explicit WebServer(unsigned short threadNum = 8);

    ~WebServer();

    WebServer &bind(unsigned short port);

    void run();

    WebServer &route(const std::string &path, HttpResponse(*callback)(HttpRequest &));

    WebServer &setStaticFilePath(const std::filesystem::path &path);

    WebServer &setTemplateFilePath(const std::filesystem::path &path);

private:
    void wrapper(int clientSocket);

    HttpResponse fileFetcher(HttpRequest &request);
};


#endif //WEBSERVER_WEBSERVER_H
