
#include "strategies/SsjsBooleanBlindStrategy.h"

const std::string &SsjsBooleanBlindStrategy::getName() const {
  static const std::string name = "SSJS Boolean Blind Strategy";
  return name;
}

const std::string &SsjsBooleanBlindStrategy::getDescription() const {
  static const std::string desc =
      "Compares the response of a known FALSE injection against a known TRUE "
      "injection. "
      "If the responses differ significantly, it confirms a boolean blind "
      "vulnerability "
      "allowing for character-by-character data extraction.";
  return desc;
}

Strategy *SsjsBooleanBlindStrategy::clone() const {
  return new SsjsBooleanBlindStrategy(*this);
}

std::vector<StrategyResult> SsjsBooleanBlindStrategy::run() {
  std::vector<StrategyResult> anomalies;

  for (const auto &value : {"a", "1", ""}) {
    for (const auto &character : Constants::Escapes) {
      for (const auto &[falseLogic, trueLogic] : Constants::BooleanBlinds) {

        std::vector<std::pair<std::string, std::string>> payloadPairs;

        for (const auto &suffix : Constants::Suffixes) {
          std::string falsePayload =
              concat(value, character, falseLogic, suffix);
          std::string truePayload =
              concat(value, character, trueLogic, suffix);
          payloadPairs.emplace_back(falsePayload, truePayload);
        }

        // Balance the syntax by ORing the rest
        std::string falsePayloadOR =
            concat(value, character, falseLogic, " || ", character, value);
        std::string truePayloadOR =
            concat(value, character, trueLogic, " || ", character, value);
        payloadPairs.emplace_back(falsePayloadOR, truePayloadOR);

        for (const auto &[falsePayload, truePayload] : payloadPairs) {
          auto falseResponse =
              this->requestBlueprint.compileRequest(falsePayload).perform();
          if (!falseResponse.has_value())
            continue;

          auto trueResponse =
              this->requestBlueprint.compileRequest(truePayload).perform();
          if (!trueResponse.has_value())
            continue;

          ResponseDiffEngine diffEngine(*falseResponse);
          double similarity = diffEngine.similarityRation(*trueResponse);
          bool statusCodeDiffers = falseResponse->get_status_code() !=
                                   trueResponse->get_status_code();

          // If there's a significant difference between the FALSE and TRUE
          // evaluations
          if (statusCodeDiffers ||
              similarity < Constants::AnomalySimilarityThreshold) {
            StrategyResultBuilder builder;
            anomalies.push_back(
                builder.baselinePayload(falsePayload)
                    .baselineResponse(*falseResponse)
                    .attackPayload(truePayload)
                    .attackResponse(*trueResponse)
                    .comment(std::format("Boolean Blind Anomaly! Status "
                                         "diff: {}, Similarity: {}%",
                                         statusCodeDiffers, similarity))
                    .build());
          }
        }
      }
    }
  }

  return anomalies;
}
