#include "WebServer.h"

int main() {
    WebServer server;
    server.bind(8080).setFilePath("./statics").run();
    return 0;
}
