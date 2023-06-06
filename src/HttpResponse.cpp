#include <iostream>
#include "HttpResponse.h"

std::filesystem::path HttpResponse::templateFilePath = "templates";

HttpResponse::HttpResponse() {
    version = "1.1";
    code = 200;
    field["Server"] = "Web Server demo by virtualfuture";
}

HttpResponse &HttpResponse::addField(const std::string &&key, const std::string &&value) {
    field[key] = value;
    return *this;
}


HttpResponse &HttpResponse::setBody(std::vector<byte> &&body_) {
    body = body_;
    return *this;
}

HttpResponse &HttpResponse::setCode(int code_) {
    code = code_;
    return *this;
}

size_t HttpResponse::build(byte *&buffer) {
    std::stringstream stream;
    stream << "HTTP/" << version << " " << code << std::endl;
    for (auto [key, value]: field) {
        stream << key << ": " << value << std::endl;
    }
    stream << "Content-Length: " << body.size() << std::endl;
    stream << std::endl;
    std::string header = stream.str();
    size_t len = header.length() + body.size();
    buffer = new byte[len];
    std::copy(header.begin(), header.end(), buffer);
    std::copy(body.begin(), body.end(), buffer + header.length());
    return len;
}

HttpResponse &HttpResponse::setBody(std::vector<byte> &body_) {
    return setBody(std::move(body_));
}

HttpResponse &HttpResponse::render(const std::filesystem::path &path, const mstch::node &data) {
    std::ifstream file(templateFilePath / path);
    if (not file.is_open()) {
        throw std::runtime_error("file not found : " + std::string(path));
    }
    std::string view(std::istreambuf_iterator<char>(file), {});
    file.close();
    std::string rendered = mstch::render(view, data);
    std::vector<byte> body_(rendered.begin(), rendered.end());
    return this->setBody(body_);
}

HttpResponse::HttpResponse(std::filesystem::path &templateFilePath_) {
    templateFilePath = templateFilePath_;
}



