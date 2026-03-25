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

class HTTPResponse {
  // Only this class is allowed to create an HTTPResponse object
  friend class CurlSession;

  int status_code = 0;
  double time = 0.0;
  HTTPHeaders headers;
  std::vector<std::byte> raw_body;
  std::string mutable body; // a string representation of raw_body

  void sync_with_raw_body() const {
    if (raw_body.size() != body.size()) {
      body = std::string(reinterpret_cast<const char*>(raw_body.data()), raw_body.size());
    }
  }

  // I know it's private. I let only CurlSession to use it
  HTTPResponse(int _status_code, double _time, const HTTPHeaders& _headers, const std::string& _body) : 
    status_code{_status_code},
    time{_time},
    headers{_headers},
    body{_body} {}
public:
  HTTPResponse() = default;

  int get_status_code() const { return status_code; }
  const std::vector<std::byte>& get_raw_body() const { return raw_body; }
  const HTTPHeaders& get_headers() const { return headers; }
  double get_time() const { return time; }
  
  // This assumes raw_body will never change
  const std::string& get_body() const {
    sync_with_raw_body();
    return body;
  }

  friend std::ostream& operator<<(std::ostream& os, const HTTPResponse& self) {
    return os << std::format("HTTPResponse {{ status={} #headers={} raw_body.size={} time={} }}", self.get_status_code(), self.get_headers().get_map().size(), self.get_raw_body().size(), self.get_time());
  }
};

class CurlSession {
  CURL *handle;


  std::string getinfo_wrapper_last_url() const {
    char* url = nullptr;
    curl_easy_getinfo(handle, CURLINFO_EFFECTIVE_URL, &url); 
    return std::string(url);
  }

  long getinfo_wrapper_last_status_code() const {
    long response_code = 0;
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &response_code);
    return response_code;
  }

  double getinfo_wrapper_total_time() const {
    double total_time = 0.0;
    curl_easy_getinfo(handle, CURLINFO_TOTAL_TIME, &total_time);
    return total_time;
  }

public:
  std::expected<HTTPResponse, CURLcode> request(const std::string& method, const std::string& url, const HTTPHeaders& _headers, const std::string& body) {
    auto headers = _headers;
    headers.remove("Accept-Encoding"); // let curl decide what encodings to accept
    HTTPResponse response;

    struct _user_data_read {
      const std::string* original_string;
      size_t position;
    } user_data_read{&body, 0};

    struct _user_data_write {
      HTTPResponse *response;
    } user_data_write{&response}, user_data_headers{&response};

    auto read_callback = [](char* buffer, size_t size, size_t nitems, void* user_data_ptr) -> size_t {
      _user_data_read* user_data = static_cast<_user_data_read*>(user_data_ptr);
      size_t ncopied = size * std::min<size_t>(nitems, user_data->original_string->size() - user_data->position); // altough size is always 1, as stated in the docs
      if (ncopied == 0) return 0;

      std::memcpy(buffer, user_data->original_string->data() + user_data->position, ncopied);
      user_data->position += ncopied;

      return ncopied;
    };

    auto write_callback = [](char* buffer, size_t size, size_t nitems, void* user_data_ptr) -> size_t {
      _user_data_write *user_data = static_cast<_user_data_write*>(user_data_ptr);
      for (size_t i = 0; i < nitems; i++) {
        user_data->response->raw_body.push_back(*reinterpret_cast<std::byte*>(buffer + i * size));
      }
      return nitems;
    };

    auto write_headers_callback = [](const char *buffer, size_t size, size_t nitems, void *userdata) -> size_t {
      _user_data_write *user_data = static_cast<_user_data_write*>(userdata);
      
      std::string header_line = "";
      for (size_t i = 0; i < nitems; i++)
        header_line += *(buffer + size * i);

      size_t pos = header_line.find(':');
      if (pos == std::string_view::npos) {
        // This means the line is either a Status or an Empty line, so we skip it
        return nitems;
      }

      std::string key = header_line.substr(0, pos);
      std::string value = header_line.substr(pos + 1);
      size_t start = value.find_first_not_of(" \t");
      value = (start == std::string_view::npos) ? "" : value.substr(start);
  
      user_data->response->headers.insert(key, value);

      return nitems;
    };



    if (!method.size())
      throw std::runtime_error("CurlSession::request: method must not be empty");
    curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, method.data());
    
    if (!url.size())
      throw std::runtime_error("CurlSession::request: url must not be empty");
    curl_easy_setopt(handle, CURLOPT_URL, url.data());

    if (headers.get_data())
      curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers.get_data());

    if (body.size()) {
      curl_easy_setopt(handle, CURLOPT_UPLOAD, 1UL);
      curl_easy_setopt(handle, CURLOPT_READFUNCTION, +read_callback); // + for explicit decay to function pointer
      curl_easy_setopt(handle, CURLOPT_READDATA, &user_data_read);
      curl_easy_setopt(handle, CURLOPT_INFILESIZE_LARGE, (curl_off_t)body.size());
    }

    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, +write_callback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &user_data_write);
    curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, +write_headers_callback);
    curl_easy_setopt(handle, CURLOPT_HEADERDATA, &user_data_headers);

    const CURLcode code = curl_easy_perform(handle);
    if (code == CURLE_OK) {
      curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &response.status_code);
      response.time = getinfo_wrapper_total_time();
    }
    
    curl_easy_setopt(handle, CURLOPT_HEADERDATA, nullptr);
    curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, nullptr);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, nullptr);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, nullptr);
    curl_easy_setopt(handle, CURLOPT_INFILESIZE_LARGE, 0UL);
    curl_easy_setopt(handle, CURLOPT_READDATA, nullptr);
    curl_easy_setopt(handle, CURLOPT_READFUNCTION, nullptr);
    curl_easy_setopt(handle, CURLOPT_UPLOAD, 0UL);
    
    if (code != CURLE_OK) 
      return std::unexpected(code);
    return response;
  }


public:
  // Let our class manage your handle. Make sure you don't use the handle anymore afterwards.
  // Useful for setting custom options.
  explicit CurlSession(CURL* _handle) : handle{_handle} {}

  CurlSession() : handle{curl_easy_init()} {
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, 20L); // Set a timeout of 20 seconds to prevent hanging
    curl_easy_setopt(handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
  }

  CurlSession(const CurlSession&) = delete;
  CurlSession(CurlSession&& other) : handle{other.handle} { other.handle = nullptr; };
  CurlSession& operator=(const CurlSession&) = delete;
  CurlSession& operator=(CurlSession&& other) {  // this must free itself and other because it basically means "overwrite this"
    if (this == &other) return *this;
    curl_easy_cleanup(this->handle);
    
    this->handle = other.handle;
    
    other.handle = nullptr;
    return *this;
  };

  ~CurlSession() {
    curl_easy_cleanup(handle);
  }

  friend std::ostream& operator<<(std::ostream& os, const CurlSession& self) {
    return os << std::format("CurlSession {{ latest_url={} latest_status_code={} latest_wait_time={} }}", self.getinfo_wrapper_last_url(), self.getinfo_wrapper_last_status_code(), self.getinfo_wrapper_total_time());
  }
};