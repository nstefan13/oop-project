#pragma once

#include <expected>
#include <iostream>
#include <string>
#include <vector>

std::string ltrim(std::string s);

std::string rtrim(std::string s);

std::string trim(const std::string &s);

std::string tolower(const std::string &s);

std::string readEverything(const std::istream &is);

std::vector<std::byte> stringToBytes(const std::string &str);

std::string removeDynamicContent(const std::string &baseResponseBody,
                                 const std::string &responseBody,
                                 size_t boundryLength);

template <typename Res, typename Err>
std::ostream &operator<<(std::ostream &os,
                         const std::expected<Res, Err> &expect) {
  if (!expect.has_value()) {
    os << "std::unexpected::Error { " << expect.error() << " }";
  } else {
    os << expect.value();
  }
  return os;
}

template <typename T> std::string concat(T str) {
  if constexpr (std::is_constructible_v<std::string, T>) {
    return std::string(str);
  } else {
    return std::to_string(str);
  }
}

template <typename T, typename... U> std::string concat(T arg1, U... other) {
  return concat(arg1) + concat(other...);
}