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

  void configurePipelines();

public:
  explicit ResponseDiffEngine(const HTTPResponse &_baselineResponse);

  double similarityRation(const HTTPResponse &httpResponse) const;

  HTTPResponse removeDynamicContentFromResponse(const HTTPResponse &response);

  ResponseComparisonResult compare(const HTTPResponse &response);
};