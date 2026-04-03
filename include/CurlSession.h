#pragma once

#include <curl/curl.h>
#include <cstring>
#include <string>
#include <ranges>
#include <iostream>
#include <expected>
#include <exception>
#include <optional>
#include <functional>
#include <memory>
#include "Utilities.h"
#include "HTTPHeaders.h"
#include "HTTPResponse.h"

class CurlSession {
  CURL *handle;

  std::string getinfo_wrapper_last_url() const;
  long getinfo_wrapper_last_status_code() const;
  double getinfo_wrapper_total_time() const;

public:
  std::expected<HTTPResponse, CURLcode> request(const std::string& method, const std::string& url, const HTTPHeaders& _headers, const std::string& body);

public:
  // Let our class manage your handle. Make sure you don't use the handle anymore afterwards.
  // Useful for setting custom options.
  explicit CurlSession(CURL* _handle) : handle{_handle} {}
  CurlSession();
  ~CurlSession();

  CurlSession(CurlSession&& other);
  CurlSession& operator=(CurlSession&& other);
  CurlSession(const CurlSession&) = delete;
  CurlSession& operator=(const CurlSession&) = delete;

  friend std::ostream& operator<<(std::ostream& os, const CurlSession& self);
};