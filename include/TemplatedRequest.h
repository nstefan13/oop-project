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
  explicit TemplatedRequest(const std::string& injaTemplate);

  explicit TemplatedRequest(const std::filesystem::path& filePath);

  HTTPRequest compileRequest(std::string payload);
};