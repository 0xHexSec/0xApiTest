#ifndef REQUEST_IO_H
#define REQUEST_IO_H

#include "http_client.h"
#include <string>

void save_request_to_file(const HttpRequest& request, const std::string& filename);
HttpRequest load_request_from_file(const std::string& filename);

#endif
