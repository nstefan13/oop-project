#pragma once

#include "Utilities.h"

#include <string>
#include <ranges>
#include <cctype>
#include <expected>
#include <format>
#include <algorithm>
#include <iostream>

std::string ltrim(std::string s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char c) {return !std::isspace(c);}));
  return s;
}

std::string rtrim(std::string s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch){ return !std::isspace(ch); }).base(), s.end());
  return s;
}

std::string trim(const std::string& s) {
  return ltrim(rtrim(s));
}

std::string tolower(const std::string& s) {
  auto view = s | std::views::transform([](unsigned char c) { return std::tolower(c); });
  return std::string(view.begin(), view.end());
}