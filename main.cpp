#include "WebServer.h"

int main() {
    YAML::Node config = YAML::LoadFile("config.yaml");
    int port = config["port"].as<int>();
    auto filePath = config["filePath"].as<std::string>();
    WebServer server;
    server.bind(port).setFilePath(filePath).run();
    return 0;
}
