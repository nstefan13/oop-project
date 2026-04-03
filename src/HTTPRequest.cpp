#include "HTTPRequest.h"

#include <stdexcept>
#include <format>
#include <algorithm>
#include <regex>
#include <expected>
#include "Utilities.h"

void HTTPRequest::parse_status_line(const std::string& status_line) {
  std::smatch matches;
  static std::regex pattern(R"(^([A-Z]+)\s+(\S+)\s+HTTP/(1|1\.1|2|3)$)");

  if (std::regex_match(status_line, matches, pattern)) {
    this->method = matches[1];
    this->path = matches[2];
  } else {
    throw std::runtime_error(std::format("HTTPRequest::parse_status_line invalid status line received: {}...", status_line.substr(0, std::min<size_t>(status_line.size(), 63UL))));
  }
}

void HTTPRequest::parse_header(const std::string& header_line) {
  std::smatch matches;
  static std::regex pattern(R"(([^:]+):\s*(.*))");

  if (std::regex_match(header_line, matches, pattern)) {
    this->headers.insert(matches[1], matches[2]);
  } else {
    throw std::runtime_error(std::format("HTTPRequest::parse_header invalid header line received: {}...", header_line.substr(0, std::min<size_t>(header_line.size(), 63UL))));
  }
}



HTTPRequest::HTTPRequest(std::istream& request_file_stream) {
  auto next_line = [&request_file_stream](){ 
    std::string line;
    std::getline(request_file_stream, line);
    return trim(line);   
  };

  std::string line;

  // Skiping empty lines
  while ((line = next_line()).size() == 0);

  // The first line is the Status Line (aka: "POST /login HTTP/2")
  parse_status_line(line);

  // The next lines until an empty line are headers
  while ((line = next_line()).size()) {
    parse_header(line);
  }

  // Store the remaining characters into this->body
  if (this->headers["Content-Length"].size()) {
    size_t content_length = std::stoull(this->headers["Content-Length"]);
    this->body.assign(std::istreambuf_iterator<char>(request_file_stream), std::istreambuf_iterator<char>());

    if (this->body.size() != static_cast<size_t>(content_length)) {
      throw std::runtime_error(std::format("HTTPRequest: Body size mismatch. Header: {}, Actual: {}", content_length, this->body.size()));
    } 
  }
}

std::string& HTTPRequest::get_method() { return method; }

std::string& HTTPRequest::get_path() { return path; }

HTTPHeaders& HTTPRequest::get_headers() { return headers; }

std::string& HTTPRequest::get_body() { return body; }

std::expected<HTTPResponse, int> HTTPRequest::perform() const {
  if (this->headers["Host"].size()) {
    CurlSession session;
    std::string scheme = "http";
    if (this->headers["Origin"].starts_with("https") || this->headers["Referer"].starts_with("https"))
      scheme = "https";

    auto url = std::format("{}://{}{}", scheme, this->headers["Host"], this->path);
    return session.request(this->method, url, this->headers, this->body);
  }
  return std::unexpected(-100);
}

std::ostream& operator<<(std::ostream& os, const HTTPRequest& self) {
  return os << std::format("HTTPRequest {{ method={} path={} #headers={} body.size={} }}", self.method, self.path, self.headers.get_map().size(), self.body.length());
}