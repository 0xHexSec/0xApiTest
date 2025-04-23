#include "http_client.h"
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <iostream>

const cpr::Timeout TIMEOUT{60000}; // 60 Sekunden

cpr::Header convert_headers(const std::map<std::string, std::string>& headers) {
    cpr::Header cprHeaders;
    for (const auto& [key, value] : headers) {
        cprHeaders[key] = value;
    }
    return cprHeaders;
}

void send_request(const HttpRequest& request) {
    auto headers = convert_headers(request.headers);
    auto body = cpr::Body{request.body};

    try
        {
            cpr::Response response;
            switch (request.method)
            {
                case HttpMethod::GET:
                    response = cpr::Get(cpr::Url{request.url}, headers, TIMEOUT);
                    break;
                case HttpMethod::POST:
                    response = cpr::Post(cpr::Url{request.url}, headers, body, TIMEOUT);
                    break;
                case HttpMethod::PUT:
                    response = cpr::Put(cpr::Url{request.url}, headers, body, TIMEOUT);
                    break;
                case HttpMethod::DELETE:
                    response = cpr::Delete(cpr::Url{request.url}, headers, TIMEOUT);
                    break;
            }

        if (response.error)
        {
            std::cerr << "❗ Netzwerkfehler: " << response.error.message << "\n";
            return;
        }

        std::cout << "Status Code: " << response.status_code << "\n";

        // JSON Content erkennen
            if (const auto contentType = response.header["content-type"]; contentType.find("application/json") != std::string::npos) {
            try {
                auto json = nlohmann::json::parse(response.text);
                std::cout << json.dump(4) << "\n"; // pretty print with indent=4
            } catch (...) {
                std::cout << "⚠️ Ungültiges JSON:\n" << response.text << "\n";
            }
        } else {
            std::cout << response.text << "\n";
        }
    } catch (const std::exception& e){
        std::cerr << "❗ Netzwerkfehler: " << e.what() << "\n";
    }
}
