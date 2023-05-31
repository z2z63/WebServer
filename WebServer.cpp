
#include "WebServer.h"


WebServer::WebServer() {
    port = -1;
    threadNum = -1;
    templatesPath = "";
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd < 0) {
        throw std::runtime_error("socket file descriptor error");
    }
    int on = 1;
    int res = setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if (res < 0) {
        throw std::runtime_error("set socket options error");
    }

}

WebServer::~WebServer() {
    close(socketFd);
}

WebServer &WebServer::bind(unsigned short _port) {
    port = _port;
    struct sockaddr_in addr{};  // address 和 name 在socket编程中是同义词
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int res = ::bind(socketFd, (struct sockaddr *) &addr, sizeof(addr));
    if (res < 0) {
        throw std::runtime_error("bind error");
    }
    return *this;
}

WebServer &WebServer::setThreadNum(unsigned short _threadNum) {
    threadNum = _threadNum;
    return *this;
}

WebServer &WebServer::run() {
    if (listen(socketFd, 16)) {
        throw std::runtime_error("listen error");
    }
    std::cout << "listening at http://127.0.0.1:" << port << "/" << std::endl;
    struct sockaddr_in clientAddr{};
    socklen_t clientAddrLen = sizeof(sockaddr_in);
    int clientSocket = -1;
    int res = -1;
    while (true) {
        clientSocket = accept(socketFd, (struct sockaddr *) &clientAddr, &clientAddrLen);
        if (clientSocket < 0) {
            throw std::runtime_error("client socket error");
        }
        wrapper(clientSocket);
        close(clientSocket);
    };
    return *this;
}

WebServer &WebServer::route(const std::string &path, void(*callback)(HttpRequest &, HttpResponse &)) {
    std::pair<std::string, void (*)(HttpRequest &, HttpResponse &)> p = {path, callback};
    routeMap.insert(p);
    return *this;
}

void WebServer::wrapper(int clientSocket) {
    HttpRequest request(clientSocket);
    HttpResponse response;
    try {
        auto it = routeMap.find(request.path);
        if (it == routeMap.end()) {
            throw std::runtime_error("path not in routeMap");
        }
        auto callback = it->second;
        callback(request, response);
    } catch (std::exception &e) {
        response.code = 404;
        std::string errorPage = "<h1>404 Not Found<h1>";
        response.body = std::vector<byte>(errorPage.begin(), errorPage.end());
        std::cout << e.what() << std::endl;
    }
    byte *buffer;
    size_t totalSize = response.build(buffer);
    size_t sentSize = 0;
    while (sentSize < totalSize) {
        size_t sent = write(clientSocket, buffer + sentSize, sizeof(byte) * (totalSize - sentSize));
        if (sent == -1) {
            throw std::runtime_error("write error");
        }
        sentSize += sent;
    }
    delete[] buffer;
}

WebServer &WebServer::setFilePath(const std::filesystem::path &path) {
    templatesPath = path;
    std::queue<std::filesystem::path> que{{templatesPath}};
    while (not que.empty()) {
        auto file = que.front();
        que.pop();
        if (std::filesystem::is_directory(file)) {
            for (const auto &f: std::filesystem::directory_iterator(file)) {
                que.push(f);
            }
            continue;
        }
        std::string urlPath = std::filesystem::relative(file, templatesPath);
        if (urlPath == "index.html") {
            routeMap["/"] = [this](HttpRequest &request, HttpResponse &response) {
                fileFetcher(request, response);
            };
        } else {
            routeMap["/" + urlPath] = [this](HttpRequest &request, HttpResponse &response) {
                fileFetcher(request, response);
            };
        }


    }
    return *this;
}

void WebServer::fileFetcher(HttpRequest &request, HttpResponse &response) {
    std::string pathWithoutSlash = request.path.substr(1, request.path.length() - 1);
    if(pathWithoutSlash.empty()){
        pathWithoutSlash = "index.html";
    }
    auto path = templatesPath / pathWithoutSlash;
    std::ifstream file(path);
    file.seekg(0, std::ios::end);
    int fileSize = (int) file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<byte> body(fileSize);
    file.read(reinterpret_cast<char *>(body.data()), fileSize);
    file.close();
    response.setBody(body);
    if(pathWithoutSlash.empty()){
        response.field["Content-Type"] = "text/html; charset=utf-8";
        return;
    }
    std::string extName = pathWithoutSlash.substr(pathWithoutSlash.rfind('.'), pathWithoutSlash.length());
    if(extName == "jpg"){
        response.field["Content-Type"] = "image/jpeg";
    }else if(extName == "png"){
        response.field["Content-Type"] = "image/png";
    }else if(extName == "css"){
        response.field["Content-Type"] = "text/css";
    }else if(extName == "js"){
        response.field["Content-Type"] = "text/javascript; charset=utf-8";
    }
}





