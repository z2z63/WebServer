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
#include "HttpResponse.h"
#include "HttpRequest.h"
#include "ThreadPool.h"


class WebServer {
private:
    unsigned short port;
    int socketFd;
    std::filesystem::path templatesPath;
    std::map<std::string, std::function<void(HttpRequest &, HttpResponse &)>> routeMap;
    ThreadPool<std::function<void()>> pool;
public:
    explicit WebServer(unsigned short threadNum = 8);

    ~WebServer();

    WebServer &bind(unsigned short port);

    void run();

    WebServer &route(const std::string &path, void(*callback)(HttpRequest &, HttpResponse &));

    WebServer &setFilePath(const std::filesystem::path &path);

private:
    void wrapper(int clientSocket);

    void fileFetcher(HttpRequest &request, HttpResponse &response);
};


#endif //WEBSERVER_WEBSERVER_H
