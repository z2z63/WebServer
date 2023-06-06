#ifndef WEBSERVER_HTTPREQUEST_H
#define WEBSERVER_HTTPREQUEST_H

#include <string>
#include <stdexcept>
#include <sstream>
#include <unistd.h>
#include <map>
#include <vector>

class HttpRequest {
public:
    std::string path;
    std::string method;
    std::string version;
    std::map<std::string, std::string> field;

    explicit HttpRequest(int clientSocket);
};


#endif //WEBSERVER_HTTPREQUEST_H
