#include "WebServer.h"

HttpResponse templateHandler(HttpRequest &request) {
    mstch::map data;
    data["time"] = std::string("111");
    return HttpResponse().render("template.html", data);
}

int main() {
    YAML::Node config = YAML::LoadFile("config.yaml");
    int port = config["port"].as<int>();
    auto staticPath = config["staticFilePath"].as<std::string>();
    auto templatePath = config["templateFilePath"].as<std::string>();
    int poolSize = config["threadPoolSize"].as<int>();
    WebServer server(poolSize);
    server.bind(port)
            .setStaticFilePath(staticPath).setTemplateFilePath(templatePath)
            .route("/template", templateHandler).run();
    return 0;
}
