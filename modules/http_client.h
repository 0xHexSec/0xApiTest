#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <string>
#include <map>

enum class HttpMethod { GET, POST, PUT, DELETE };

struct HttpRequest {
    std::string url;
    HttpMethod method;
    std::map<std::string, std::string> headers;
    std::string body;
};

void send_request(const HttpRequest& request);

#endif // HTTP_CLIENT_H
