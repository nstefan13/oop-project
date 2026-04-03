#pragma once

#include <string>
#include <expected>
#include <iostream>

std::string ltrim(std::string s);

std::string rtrim(std::string s);

std::string trim(const std::string& s);

std::string tolower(const std::string& s);

std::string readEverything(const std::istream& is);

template<typename Res, typename Err>
std::ostream& operator<<(std::ostream& os, const std::expected<Res, Err>& expect) {
  if (!expect.has_value()) {
    os << "std::unexpected::Error { " << expect.error() << " }"; 
  } else {
    os << expect.value();
  }
  return os;
}