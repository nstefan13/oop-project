#pragma once

#include "HTTPHeaders.h"
#include <vector>
#include <cstddef>

class HTTPResponse {
  // Only this class is allowed to create an HTTPResponse object
  friend class CurlSession;

  int status_code = 0;
  double time = 0.0;
  HTTPHeaders headers;
  std::vector<std::byte> raw_body;
  std::string mutable body; // a string representation of raw_body

  void sync_with_raw_body() const;

  // I know it's private. I only allow CurlSession use it
  HTTPResponse(int _status_code, double _time, const HTTPHeaders& _headers, const std::string& _body);
public:
  HTTPResponse() = default;

  int get_status_code() const;
  const std::vector<std::byte>& get_raw_body() const;
  const HTTPHeaders& get_headers() const;
  double get_time() const;
  
  // This assumes raw_body will never change
  const std::string& get_body() const;

  friend std::ostream& operator<<(std::ostream& os, const HTTPResponse& self);
};