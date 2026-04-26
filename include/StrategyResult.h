#pragma once

#include "HTTPRequest.h"
#include "HTTPResponse.h"
#include "Strategy.h"
#include <optional>
#include <string>

// Contains all the necessary context to understand why a specific payload
// triggered an anomaly compared to the baseline.
class StrategyResult {
private:
  std::string baselinePayload;
  HTTPResponse baselineResponse;

  std::string attackPayload;
  HTTPResponse attackResponse;

  std::string
      anomalyComment; // Explanation of why this result is considered abnormal

  // Only the builder can "build" this
  friend class StrategyResultBuilder;

  // Private constructor used by the Builder
  StrategyResult() = default;

public:
  [[nodiscard]] const std::string &getBaselinePayload() const {
    return baselinePayload;
  }
  [[nodiscard]] const HTTPResponse &getBaselineResponse() const {
    return baselineResponse;
  }
  [[nodiscard]] const std::string &getAttackPayload() const {
    return attackPayload;
  }
  [[nodiscard]] const HTTPResponse &getAttackResponse() const {
    return attackResponse;
  }
  [[nodiscard]] const std::string &getAnomalyComment() const {
    return anomalyComment;
  }
};

class StrategyResultBuilder {
  StrategyResult result;

public:
  [[nodiscard]] StrategyResultBuilder &
  baselinePayload(const std::string &payload) {
    result.baselinePayload = payload;
    return *this;
  }

  [[nodiscard]] StrategyResultBuilder &
  baselineResponse(const HTTPResponse &response) {
    result.baselineResponse = response;
    return *this;
  }

  [[nodiscard]] StrategyResultBuilder &
  attackPayload(const std::string &payload) {
    result.attackPayload = payload;
    return *this;
  }

  [[nodiscard]] StrategyResultBuilder &
  attackResponse(const HTTPResponse &response) {
    result.attackResponse = response;
    return *this;
  }

  [[nodiscard]] StrategyResultBuilder &comment(const std::string &comment) {
    result.anomalyComment = comment;
    return *this;
  }

  StrategyResult build() {
    auto res = std::move(result);

    result = StrategyResult(); // reset the builder state

    return res;
  }
};