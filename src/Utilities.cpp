#include "Utilities.h"

#include <algorithm>
#include <cctype>
#include <expected>
#include <format>
#include <iostream>
#include <ranges>
#include <rapidfuzz/distance/Levenshtein.hpp>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

std::string ltrim(std::string s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char c) {
            return !std::isspace(c);
          }));
  return s;
}

std::string rtrim(std::string s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char ch) { return !std::isspace(ch); })
              .base(),
          s.end());
  return s;
}

std::string trim(const std::string &s) { return ltrim(rtrim(s)); }

std::string readEverything(const std::istream &is) {
  std::ostringstream oss;
  oss << is.rdbuf();
  return oss.str();
}

std::string tolower(const std::string &s) {
  auto view = s | std::views::transform(
                      [](unsigned char c) { return std::tolower(c); });
  return std::string(view.begin(), view.end());
}

std::vector<std::byte> stringToBytes(const std::string &str) {
  const auto *byteData = reinterpret_cast<const std::byte *>(str.data());
  return std::vector<std::byte>(byteData, byteData + str.size());
}

std::string removeDynamicContent(const std::string &baseResponseBody,
                                 const std::string &responseBody,
                                 size_t boundryLength) {
  if (baseResponseBody.empty() || responseBody.empty()) {
    return responseBody;
  }

  rapidfuzz::Editops editops =
      rapidfuzz::levenshtein_editops(baseResponseBody, responseBody);

  std::string filteredBody;
  size_t current_dest = 0;

  for (const auto &op : editops) {
    if (op.dest_pos > current_dest) {
      size_t match_length = op.dest_pos - current_dest;
      if (match_length > boundryLength) {
        filteredBody += responseBody.substr(current_dest, match_length);
      }
    }

    if (op.type == rapidfuzz::EditType::Replace) {
      current_dest = op.dest_pos + 1;
    } else if (op.type == rapidfuzz::EditType::Insert) {
      current_dest = op.dest_pos + 1;
    } else if (op.type == rapidfuzz::EditType::Delete) {
      current_dest = op.dest_pos;
    }
  }

  if (responseBody.length() > current_dest) {
    size_t match_length = responseBody.length() - current_dest;
    if (match_length > boundryLength) {
      filteredBody += responseBody.substr(current_dest, match_length);
    }
  }

  return filteredBody;
}
