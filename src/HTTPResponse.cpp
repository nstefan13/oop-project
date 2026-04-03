#include "HTTPResponse.h"

void HTTPResponse::sync_with_raw_body() const {
  if (raw_body.size() != body.size()) {
    body = std::string(reinterpret_cast<const char*>(raw_body.data()), raw_body.size());
  }
}

// I know it's private. I let only CurlSession to use it
HTTPResponse::HTTPResponse(int _status_code, double _time, const HTTPHeaders& _headers, const std::string& _body) : 
  status_code{_status_code},
  time{_time},
  headers{_headers},
  body{_body} {};

int HTTPResponse::get_status_code() const { return status_code; }
const std::vector<std::byte>& HTTPResponse::get_raw_body() const { return raw_body; }
const HTTPHeaders& HTTPResponse::get_headers() const { return headers; }
double HTTPResponse::get_time() const { return time; }

// This assumes raw_body will never change
const std::string& HTTPResponse::get_body() const {
  sync_with_raw_body();
  return body;
}

std::ostream& operator<<(std::ostream& os, const HTTPResponse& self) {
  return os << std::format("HTTPResponse {{ status={} #headers={} raw_body.size={} time={} }}", self.get_status_code(), self.get_headers().get_map().size(), self.get_raw_body().size(), self.get_time());
}
