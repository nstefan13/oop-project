#include "HTTPHeaders.h"

#include <string>
#include <iostream>
#include <format>
#include <unordered_map>
#include <curl/curl.h>
#include "Utilities.h"

std::string HTTPHeaders::normalize(const std::string& header_name) {
  return tolower(header_name);
}

std::string HTTPHeaders::format_header(const std::string& name, const std::string& value) {
  if (!value.size()) return name + ";";
  else return std::format("{}: {}", normalize(name), value);
}

const curl_slist* HTTPHeaders::get_data() const {
  if (this->headers_version != this->data_version) {
    this->data_version = this->headers_version;

    curl_slist_free_all(data);
    this->data = nullptr;
    for (auto&& [header_name, header_value] : this->headers) {
      this->data = curl_slist_append(this->data, format_header(header_name, header_value).data());
    }
  }

  return data;
}

const std::unordered_map<std::string, std::string>& HTTPHeaders::get_map() const { return headers; }

void HTTPHeaders::insert(const std::string& name, const std::string& value) {
  this->headers_version++;
  headers[normalize(name)] = value;
}

void HTTPHeaders::remove(const std::string& name) {
  this->headers_version++;
  if (headers.count(normalize(name)))
    headers.erase(normalize(name));
}

const std::string& HTTPHeaders::operator[](const std::string& header_name) const {
  static const std::string empty_string;
  if (!headers.count(normalize(header_name))) {
    return empty_string;
  }
  return headers.at(normalize(header_name));
}




HTTPHeaders::HTTPHeaders(const std::initializer_list<std::pair<std::string, std::string>>& initial_headers) {
  this->headers_version = this->data_version = 0;
  this->data = nullptr;

  for (auto&& [header_name, header_value] : initial_headers) {
    headers[normalize(header_name)] = header_value;
    this->data = curl_slist_append(data, format_header(header_name, header_value).data());
  }
}

HTTPHeaders::HTTPHeaders(const HTTPHeaders& other) : headers{other.headers}, headers_version{0}, data{nullptr}, data_version{0} {
  for (auto&& [name, value] : other.headers) {
    this->data = curl_slist_append(data, format_header(name, value).data());
  }
}

HTTPHeaders::HTTPHeaders(HTTPHeaders&& other) : 
  headers{std::move(other.headers)},
  headers_version{other.headers_version}, 
  data{other.data},
  data_version{other.data_version} {
    other.data = nullptr;
}

HTTPHeaders& HTTPHeaders::operator=(const HTTPHeaders& other) {
  if (this == &other) return *this;
  headers = other.headers;
  this->data_version = this->headers_version = 0;
  
  curl_slist *new_data = nullptr;
  for (auto&& [name, value] : other.headers) {
    new_data = curl_slist_append(new_data, format_header(name, value).data());
  }

  curl_slist_free_all(this->data);
  this->data = new_data;

  return *this;
}

HTTPHeaders& HTTPHeaders::operator=(HTTPHeaders&& other) {
  if (this == &other) return *this;
  this->headers_version = other.headers_version;
  this->data_version = other.data_version;
  this->headers = std::move(other.headers);
  
  // ALWAYS set the pointers to nullptr when moving
  curl_slist_free_all(this->data);
  this->data = other.data;
  other.data = nullptr;
  return *this;
}

HTTPHeaders::~HTTPHeaders() {
  this->headers_version = this->data_version = 0;
  this->headers.clear();
  curl_slist_free_all(this->data);
  this->data = nullptr;
}

std::ostream& operator<<(std::ostream& os, const HTTPHeaders& self) {
  return os << "HTTPHeaders { count=" << self.headers.size() << " }";
}