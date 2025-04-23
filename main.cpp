#include <algorithm>
#include <cstdlib>
#include "modules/http_client.h"
#include <iostream>

int main() {
    HttpRequest request;

    std::string methodStr;
    std::cout << "HTTP-Methode (GET, POST, PUT, DELETE): ";
    std::getline(std::cin, methodStr);

    std::ranges::transform(methodStr.begin(), methodStr.end(), methodStr.begin(),
    [](const unsigned char c){ return std::tolower(c); });


    if (methodStr == "get") request.method = HttpMethod::GET;
    else if (methodStr == "post") request.method = HttpMethod::POST;
    else if (methodStr == "put") request.method = HttpMethod::PUT;
    else if (methodStr == "delete") request.method = HttpMethod::DELETE;
    else {
        std::cerr << "Ungültige Methode\n";
        return EXIT_FAILURE;
    }

    std::cout << "URL: ";
    std::getline(std::cin, request.url);

    if (request.url.empty() || request.url.find("http") != 0) {
        std::cerr << "❌ Ungültige URL.\n";
        return EXIT_FAILURE;
    }

    std::cout << "Header eingeben? (key:value, leer lassen zum Beenden):\n";
    while (true) {
        std::string line;
        std::getline(std::cin, line);
        if (line.empty()) break;

        if (const size_t delim = line.find(':'); delim != std::string::npos) {
            std::string key = line.substr(0, delim);
            const std::string value = line.substr(delim + 1);
            request.headers[key] = value;
        }
    }

    std::cout << "Body (für POST/PUT): ";
    std::getline(std::cin, request.body);

    send_request(request);

    return EXIT_SUCCESS;
}
