
#include "strategies/SsjsTimeBlindStrategy.h"

const std::string &SsjsTimeBlindStrategy::getName() const {
  static const std::string name = "SSJS Time-Based Blind Strategy";
  return name;
}

const std::string &SsjsTimeBlindStrategy::getDescription() const {
  static const std::string desc =
      "Injects busy-wait loops to measure response times. "
      "Useful when the application suppresses error messages or returns "
      "the same output regardless of the query outcome.";
  return desc;
}

Strategy *SsjsTimeBlindStrategy::clone() const {
  return new SsjsTimeBlindStrategy(*this);
}

std::vector<StrategyResult> SsjsTimeBlindStrategy::run() {
  std::vector<StrategyResult> anomalies;

  auto baselineResponse =
      this->requestBlueprint.compileRequest("dummyUsername").perform();
  if (!baselineResponse.has_value()) {
    std::cerr << "Baseline request failed with curl code: " << baselineResponse.error() << "\n";
    return anomalies;
  }

  std::vector<std::pair<std::string, int>> payloads;

  for (const auto &value : {"a", "1", ""}) {
    for (const auto &character : Constants::Escapes) {
      for (const auto &timeDelayFormat : Constants::TimeDelays) {
        for (int delayMs : {1000, 2500}) {
          std::string timeDelay;
          try {
            timeDelay =
                std::vformat(timeDelayFormat, std::make_format_args(delayMs));
          } catch (const std::format_error &e) {
            std::cerr << "Format error: " << e.what() << "\n";
            continue;
          }

          for (const auto &suffix : Constants::Suffixes) {
            payloads.push_back(
                {concat(value, character, timeDelay, suffix), delayMs});
          }
          // Balance the syntax by ORing the rest
          payloads.push_back(
              {concat(value, character, timeDelay, " || ", character, value),
               delayMs});
        }
      }
    }
  }

  for (const auto &[payload, delayMs] : payloads) {
    auto response = this->requestBlueprint.compileRequest(payload).perform();
    if (!response.has_value())
      continue;

    double timeDiff = response->get_time() - baselineResponse->get_time();
    double expectedDelaySec = delayMs / 1000.0;

    // Check if it took at least as long as the injected delay
    if (timeDiff >= expectedDelaySec - 0.1) {
      StrategyResultBuilder builder;
      anomalies.push_back(builder.baselinePayload("dummyUsername")
          .baselineResponse(*baselineResponse)
          .attackPayload(payload)
          .attackResponse(*response)
          .comment(std::format("Time Blind Anomaly! Expected > {:.2f}s, took "
                               "{:.2f}s longer than baseline.",
                               expectedDelaySec, timeDiff))
          .build());
    }
  }

  return anomalies;
}
