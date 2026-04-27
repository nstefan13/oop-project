
#include "include/CurlSession.h"
#include "include/Exceptions.hpp"
#include "include/HTTPRequest.h"
#include "include/StrategyRegistry.h"
#include "include/TemplatedRequest.h"
#include "include/Utilities.h"
#include "include/strategies/DummyStrategy.h"
#include <curl/curl.h>
#include <inja/inja.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

#include "include/strategies/OperatorInjectionStrategy.h"
#include "include/strategies/SsjsBooleanBlindStrategy.h"
#include "include/strategies/SsjsTimeBlindStrategy.h"

int main(void) {
  try {
    StrategyRegistry &registry = StrategyRegistry::getInstance();

    std::cout << "\nProcessing request template from std::cin...\n";
    std::string sablon = readEverything(std::cin);
    TemplatedRequest treq(sablon);

    std::cout << "Registering strategies...\n";
    registry.addStrategy(std::make_unique<SsjsTimeBlindStrategy>(treq));
    registry.addStrategy(std::make_unique<SsjsBooleanBlindStrategy>(treq));
    registry.addStrategy(std::make_unique<OperatorInjectionStrategy>(treq));

    std::cout << "Performing initial baseline connection check...\n";
    auto baselineCheck = treq.compileRequest("dummyUsername").perform();
    if (!baselineCheck.has_value()) {
      throw ApplicationException("Server unreachable. Curl failed with code: " +
                                 std::to_string(baselineCheck.error()));
    }

    std::cout << "Starting Fuzzer Engine...\n\n";
    auto anomalies = registry.runAll();

    std::cout << "\n========== FUZZING COMPLETE ==========\n";
    std::cout << "Anomalies detected: " << anomalies.size() << "\n";
    for (const auto &anomaly : anomalies) {
      std::cout << "- " << anomaly.getAnomalyComment() << " --->"
                << anomaly.getAttackPayload() << "\n";
    }
  } catch (const std::exception &e) {
    std::cerr << "Fatal Error: " << e.what() << "\n";
    return 0;
  }
  return 0;
}
