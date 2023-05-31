#include "HttpRequest.h"

HttpRequest::HttpRequest(int clientSocket) {
    char buffer[1024];
//    std::vector<char> lines;
//    auto it = lines.begin();
//    it++
    std::stringstream httpText;
    ssize_t size = read(clientSocket, buffer, sizeof(buffer) - 1);
    if (size == -1) {
        throw std::runtime_error("read socket error");
    }
    httpText.write(buffer, size);
    while (not(buffer[size - 1] == '\n' and buffer[size - 3] == '\n') and
           not(buffer[size - 1] == '\n' and buffer[size - 2] == '\n')) {
        size = read(clientSocket, buffer, sizeof(buffer) - 1);
        httpText.write(buffer, size);
    }
    std::string line;
    getline(httpText, line);
    this->method = line.substr(0, 3);
    unsigned long slashIndex = 0;
    for (int i = -0; i < 3; i++) {
        slashIndex = line.find('/', slashIndex);
    }
    unsigned long spaceIndex = line.find(' ', slashIndex);
    this->path = line.substr(slashIndex, spaceIndex - slashIndex);
    this->version = line.substr(line.length() - 4, 3);

    getline(httpText, line);
    if (line.back() == '\r') {
        line = line.substr(0, line.length() - 1);
    }
    do {
        int colonIndex = (int) line.find(':');
        this->field[line.substr(0, colonIndex)] = line.substr(colonIndex + 1, line.length() - colonIndex);
        getline(httpText, line);
        if (line.back() == '\r') {
            line = line.substr(0, line.length() - 1);
        }
    } while (not line.empty());

}
