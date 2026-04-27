#pragma once

#include "../Constants.h"
#include "ResponseDiffEngine.h"
#include "Strategy.h"
#include <format>
#include <iostream>
#include <string>
#include <vector>

class OperatorInjectionStrategy : public Strategy {
public:
  explicit OperatorInjectionStrategy(const TemplatedRequest &requestBlueprint)
      : Strategy(requestBlueprint) {}

  [[nodiscard]] const std::string &getName() const override {
    static const std::string name = "Operator Injection Strategy";
    return name;
  }

  [[nodiscard]] const std::string &getDescription() const override {
    static const std::string desc =
        "Attempts to inject NoSQL operators into the payload. "
        "The payload is a JSON object with "
        "operators like $ne, $gt, $regex, or $nin. This is designed to bypass "
        "weak "
        "type checking in APIs (such as those using express.json()).";
    return desc;
  }

  [[nodiscard]] Strategy *clone() const override {
    return new OperatorInjectionStrategy(*this);
  }

  std::vector<StrategyResult> run() override {
    std::vector<StrategyResult> anomalies;

    auto baselineResponse =
        this->requestBlueprint.compileRequest("dummyUsername").perform();
    if (!baselineResponse.has_value()) {
      std::cerr << "Baseline request failed.\n";
      return anomalies;
    }

    ResponseDiffEngine diffEngine(*baselineResponse);

    for (const auto &payload : Constants::OperatorInjections) {
      auto response = this->requestBlueprint.compileRequest(payload).perform();
      if (!response.has_value())
        continue;

      double similarity = diffEngine.similarityRation(*response);
      bool statusCodeDiffers =
          response->get_status_code() != baselineResponse->get_status_code();

      if (statusCodeDiffers ||
          similarity < Constants::AnomalySimilarityThreshold) {
        StrategyResultBuilder builder;
        anomalies.push_back(
            builder.baselinePayload("dummyUsername")
                .baselineResponse(*baselineResponse)
                .attackPayload(payload)
                .attackResponse(*response)
                .comment(std::format("Operator Injection Anomaly! Status diff: "
                                     "{}, Similarity: {}%",
                                     statusCodeDiffers, similarity))
                .build());
      }
    }

    return anomalies;
  }
};
