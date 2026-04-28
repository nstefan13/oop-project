#include "TemplatedRequest.h"

TemplatedRequest::TemplatedRequest(const std::string& injaTemplate) {
  this->injaTemplate = environment.parse(injaTemplate);
}

TemplatedRequest::TemplatedRequest(const std::filesystem::path& filePath) {
  this->injaTemplate = environment.parse_file(filePath);
}

HTTPRequest TemplatedRequest::compileRequest(std::string payload) {
  nlohmann::json data = {
    {"payload", payload}
  };

  std::string raw_request = environment.render(injaTemplate, data);
  std::istringstream isstream(raw_request);
  return HTTPRequest(isstream);
}
