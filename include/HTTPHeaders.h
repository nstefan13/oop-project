#pragma once

#include <string>
#include <iostream>
#include <format>
#include <unordered_map>
#include <curl/curl.h>
#include "Utilities.h"

class HTTPHeaders {
  std::unordered_map<std::string, std::string> headers;
  int headers_version;
  mutable curl_slist* data;
  mutable int data_version;

  static std::string normalize(const std::string& header_name);
  static std::string format_header(const std::string& name, const std::string& value);

public:
  const curl_slist* get_data() const;
  const std::unordered_map<std::string, std::string>& get_map() const;

  void insert(const std::string& name, const std::string& value);
  void remove(const std::string& name);
  const std::string& operator[](const std::string& header_name) const;

public:
  HTTPHeaders() : headers{}, headers_version{0}, data{nullptr}, data_version{0} {}
  explicit(false) HTTPHeaders(const std::initializer_list<std::pair<std::string, std::string>>& initial_headers);
  
  HTTPHeaders(const HTTPHeaders& other);
  HTTPHeaders(HTTPHeaders&& other);
  HTTPHeaders& operator=(const HTTPHeaders& other);
  HTTPHeaders& operator=(HTTPHeaders&& other);
  ~HTTPHeaders();

  friend std::ostream& operator<<(std::ostream& os, const HTTPHeaders& self);
};