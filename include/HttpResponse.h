#ifndef WEBSERVER_HTTPRESPONSE_H
#define WEBSERVER_HTTPRESPONSE_H

#include <map>
#include <string>
#include <sstream>
#include <unistd.h>
#include <stdexcept>
#include <vector>
#include <filesystem>
#include <fstream>
#include <mstch/mstch.hpp>

typedef unsigned char byte;

class HttpResponse {
public:
    std::map<std::string, std::string> field;
    std::vector<byte> body;
    std::string version;
    int code;
    static std::filesystem::path templateFilePath;

    HttpResponse();

    explicit HttpResponse(std::filesystem::path& templateFilePath);

    HttpResponse &addField(const std::string &&key, const std::string &&value);


    HttpResponse &setBody(std::vector<byte> &&body_);

    HttpResponse &setBody(std::vector<byte> &body_);

    HttpResponse &setCode(int code_);

    HttpResponse& render(const std::filesystem::path &path, const mstch::node &data);

    size_t build(byte *&buffer);
};


#endif //WEBSERVER_HTTPRESPONSE_H
