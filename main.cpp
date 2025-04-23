#include <algorithm>
#include "modules/http_client.h"
#include "modules/request_io.h"
#include <iostream>
#include <filesystem>

int main() {
    std::cout << "📡 0xApiTest CLI\n";
    std::cout << "[1] Neuen Request eingeben\n";
    std::cout << "[2] Gespeicherten Request laden\n";
    std::cout << "> ";

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    HttpRequest request;

    if (choice == 1) {
        std::string methodStr;
        std::cout << "HTTP-Methode (GET, POST, PUT, DELETE): ";
        std::getline(std::cin, methodStr);

        if (methodStr.empty()) {
            std::cerr << "Methode darf nicht leer sein\n";
            return 1;
        }

        std::ranges::transform(methodStr, methodStr.begin(), ::toupper);

        if (methodStr == "GET") request.method = HttpMethod::GET;
        else if (methodStr == "POST") request.method = HttpMethod::POST;
        else if (methodStr == "PUT") request.method = HttpMethod::PUT;
        else if (methodStr == "DELETE") request.method = HttpMethod::DELETE;
        else {
            std::cerr << "Ungültige Methode\n";
            return 1;
        }

        std::cout << "URL: ";
        std::getline(std::cin, request.url);

        std::cout << "Header eingeben? (key:value, leer lassen zum Beenden):\n";
        while (true) {
            std::string line;
            std::getline(std::cin, line);
            if (line.empty()) break;

            size_t delim = line.find(':');
            if (delim != std::string::npos) {
                std::string key = line.substr(0, delim);
                std::string value = line.substr(delim + 1);
                request.headers[key] = value;
            }
        }

        if (request.method == HttpMethod::POST || request.method == HttpMethod::PUT)
        {
            std::cout << "Body (für POST/PUT): ";
            std::getline(std::cin, request.body);

        }

        send_request(request);

        std::string save;
        std::cout << "Möchtest du den Request speichern? (J/n): ";
        std::getline(std::cin, save);
        std::ranges::transform(save, save.begin(), ::tolower);
        if (save != "n") {
            std::string filename;
            std::cout << "Dateiname (z.B. test_request.json): ";
            std::getline(std::cin, filename);
            save_request_to_file(request, filename);
        }

    } else if (choice == 2) {
        std::string filename;
        std::cout << "Dateiname aus saved_requests/: ";
        std::getline(std::cin, filename);
        try {
            request = load_request_from_file(filename);
            send_request(request);
        } catch (const std::exception& ex) {
            std::cerr << ex.what() << "\n";
        }
    }

    return 0;
}
