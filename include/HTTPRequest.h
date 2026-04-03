#pragma once

#include <string>
#include <iostream>
#include "HTTPHeaders.h"
#include "HTTPResponse.h"
#include "CurlSession.h"

class HTTPRequest {
  std::string method;
  std::string path;
  HTTPHeaders headers;
  std::string body;

  void parse_status_line(const std::string& status_line);
  void parse_header(const std::string& header_line);

public:
  HTTPRequest() = default;
  explicit HTTPRequest(std::istream& request_file_stream);

  std::string& get_method();
  std::string& get_path();
  HTTPHeaders& get_headers();
  std::string& get_body();

  std::expected<HTTPResponse, int> perform() const;

  friend std::ostream& operator<<(std::ostream& os, const HTTPRequest& self);
};