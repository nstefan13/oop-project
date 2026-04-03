#pragma once

#include <string>
#include <filesystem>
#include <inja/inja.hpp>
#include <nlohmann/json.hpp>
#include "HTTPRequest.h"

class TemplatedRequest {
  inja::Environment environment;
  inja::Template injaTemplate;

public:
  explicit TemplatedRequest(const std::string& injaTemplate) {
    this->injaTemplate = environment.parse(injaTemplate);
  }

  explicit TemplatedRequest(const std::filesystem::path& filePath) {
    this->injaTemplate = environment.parse_file(filePath);
  }

  HTTPRequest compileRequest(std::string payload) {
    nlohmann::json data = {
      {"payload", payload}
    };

    std::string raw_request = environment.render(injaTemplate, data);
    std::istringstream isstream(raw_request);
    return HTTPRequest(isstream);
  }
};