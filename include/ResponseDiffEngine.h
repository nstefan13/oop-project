#pragma once

#include "HTTPResponse.h"
#include "Utilities.h"
#include "rapidfuzz/distance/Levenshtein.hpp"
#include <cstddef>
#include <rapidfuzz/fuzz.hpp>
#include <vector>

enum ResponseComparisonResult { VULNERABLE, MAYBE_VULNERABLE, NOT_VULNERABLE };

class ResponseDiffEngine {
  HTTPResponse baselineResponse;

public:
  explicit ResponseDiffEngine(const HTTPResponse &_baselineResponse)
      : baselineResponse{_baselineResponse} {}

  double similarityRation(const HTTPResponse &httpResponse) const {
    return rapidfuzz::fuzz::ratio(baselineResponse.get_body(),
                                  httpResponse.get_body());
  }

  const HTTPResponse
  removeDynamicContentFromResponse(const HTTPResponse &response) const {
    if (response.get_body().empty() || baselineResponse.get_body().empty())
      return response;

    HTTPResponse modifiedResponse(response);

    std::string filteredBody = removeDynamicContent(baselineResponse.get_body(),
                                                    response.get_body(), 40);

    const auto *byteData =
        reinterpret_cast<const std::byte *>(filteredBody.data());

    modifiedResponse.raw_body =
        std::vector<std::byte>(byteData, byteData + filteredBody.size());

    modifiedResponse.sync_with_raw_body();

    return modifiedResponse;
  }

  ResponseComparisonResult compare(const HTTPResponse &response) {
    auto &modifiedResponse = removeDynamicContentFromResponse(response);
    double ratio = similarityRation(modifiedResponse);
    bool statusCodesDiffer =
        baselineResponse.get_status_code() != response.get_status_code();
  }
};