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
#include "HttpResponse.h"
#include "HttpRequest.h"


class WebServer {
private:
    unsigned short port;
    unsigned short threadNum;
    int socketFd;
    std::filesystem::path templatesPath;
    std::map<std::string, std::function<void(HttpRequest &, HttpResponse &)>> routeMap;
public:
    WebServer();

    ~WebServer();

    WebServer &bind(unsigned short port);

    WebServer &setThreadNum(unsigned short _threadNum);

    WebServer &run();

    WebServer &route(const std::string &path, void(*callback)(HttpRequest &, HttpResponse &));

    WebServer &setFilePath(const std::filesystem::path &path);

private:
    void wrapper(int clientSocket);

    void fileFetcher(HttpRequest &request, HttpResponse &response);
};


#endif //WEBSERVER_WEBSERVER_H
