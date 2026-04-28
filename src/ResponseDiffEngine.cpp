#include "ResponseDiffEngine.h"

void ResponseDiffEngine::configurePipelines() {
  stringPipeline.addStep([](const std::string &body) { return trim(body); });
  stringPipeline.addStep([this](const std::string &body) {
    return removeDynamicContent(this->baselineResponse.get_body(), body, 40);
  });

  responsePipeline.addStep([this](HTTPResponse response) {
    if (response.get_body().empty() ||
        this->baselineResponse.get_body().empty())
      return response;

    std::string filteredBody =
        this->stringPipeline.apply(response.get_body());
    const auto *byteData =
        reinterpret_cast<const std::byte *>(filteredBody.data());

    response.raw_body =
        std::vector<std::byte>(byteData, byteData + filteredBody.size());
    response.sync_with_raw_body();
    return response;
  });
}

ResponseDiffEngine::ResponseDiffEngine(const HTTPResponse &_baselineResponse)
    : baselineResponse{_baselineResponse} {
  configurePipelines();
}

double ResponseDiffEngine::similarityRation(const HTTPResponse &httpResponse) const {
  return rapidfuzz::fuzz::ratio(baselineResponse.get_body(),
                                httpResponse.get_body());
}

HTTPResponse ResponseDiffEngine::removeDynamicContentFromResponse(const HTTPResponse &response) {
  return responsePipeline.apply(response);
}

ResponseComparisonResult ResponseDiffEngine::compare(const HTTPResponse &response) {
  auto modifiedResponse = removeDynamicContentFromResponse(response);
  double ratio = similarityRation(modifiedResponse);
  bool statusCodesDiffer =
      baselineResponse.get_status_code() != response.get_status_code();

  if (statusCodesDiffer || ratio < 85.0)
    return VULNERABLE;
  return NOT_VULNERABLE;
}
