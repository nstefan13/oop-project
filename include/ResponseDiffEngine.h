#pragma once

#include "HTTPResponse.h"
#include "Pipeline.h"
#include "Utilities.h"
#include "rapidfuzz/distance/Levenshtein.hpp"
#include <cstddef>
#include <rapidfuzz/fuzz.hpp>
#include <vector>

enum ResponseComparisonResult { VULNERABLE, MAYBE_VULNERABLE, NOT_VULNERABLE };

class ResponseDiffEngine {
  HTTPResponse baselineResponse;
  Pipeline<std::string> stringPipeline;
  Pipeline<HTTPResponse> responsePipeline;

  void configurePipelines() {
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

public:
  explicit ResponseDiffEngine(const HTTPResponse &_baselineResponse)
      : baselineResponse{_baselineResponse} {
    configurePipelines();
  }

  double similarityRation(const HTTPResponse &httpResponse) const {
    return rapidfuzz::fuzz::ratio(baselineResponse.get_body(),
                                  httpResponse.get_body());
  }

  HTTPResponse removeDynamicContentFromResponse(const HTTPResponse &response) {
    return responsePipeline.apply(response);
  }

  ResponseComparisonResult compare(const HTTPResponse &response) {
    auto modifiedResponse = removeDynamicContentFromResponse(response);
    double ratio = similarityRation(modifiedResponse);
    bool statusCodesDiffer =
        baselineResponse.get_status_code() != response.get_status_code();

    if (statusCodesDiffer || ratio < 85.0)
      return VULNERABLE;
    return NOT_VULNERABLE;
  }
};