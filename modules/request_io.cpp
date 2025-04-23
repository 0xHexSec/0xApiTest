#include "request_io.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <filesystem>

void save_request_to_file(const HttpRequest& request, const std::string& filename) {
    std::filesystem::create_directories("saved_requests");

    std::string fullPath = "saved_requests/" + filename;
    if (!filename.ends_with(".json")) {
        fullPath += ".json";
    }

    nlohmann::json j;
    j["url"] = request.url;
    j["method"] = static_cast<int>(request.method);
    j["headers"] = request.headers;
    j["body"] = request.body;

    std::ofstream out(fullPath);
    if (!out.is_open()) {
        std::cerr << "❌ Fehler beim Öffnen der Datei zum Speichern: " << fullPath << "\n";
        return;
    }

    out << j.dump(4);
    std::cout << "💾 Gespeichert unter: " << fullPath << "\n";
}


HttpRequest load_request_from_file(const std::string& filename) {
    std::string fullPath = "saved_requests/" + filename;
    if (!filename.ends_with(".json")) {
        fullPath += ".json";
    }

    std::ifstream in(fullPath);
    if (!in.is_open()) {
        throw std::runtime_error("❌ Datei konnte nicht geöffnet werden: " + fullPath);
    }

    nlohmann::json j;
    in >> j;

    HttpRequest req;
    req.url = j["url"].get<std::string>();
    req.method = static_cast<HttpMethod>(j["method"].get<int>());
    req.headers = j["headers"].get<std::map<std::string, std::string>>();
    req.body = j["body"].get<std::string>();
    return req;
}

