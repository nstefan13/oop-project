#pragma once

#include "strategies/Strategy.h"
#include <iostream>
#include <memory>
#include <vector>

class StrategyRegistry {
private:
  std::vector<std::unique_ptr<Strategy>> strategies;
  StrategyRegistry() = default;
  static StrategyRegistry *instance;

public:
  static StrategyRegistry &getInstance();

  void addStrategy(std::unique_ptr<Strategy> strategy);

  std::vector<StrategyResult> runAll();

  template <typename SpecificStrategyType>
  std::vector<StrategyResult> runOnly() {
    std::vector<StrategyResult> specificAnomalies;
    for (const auto &strat : strategies) {
      if (auto specificStrat =
              dynamic_cast<SpecificStrategyType *>(strat.get())) {
        std::cout << "[*] Running SPECIFIC strategy: "
                  << specificStrat->getName() << "\n";
        auto results = specificStrat->run();
        specificAnomalies.insert(specificAnomalies.end(), results.begin(),
                                 results.end());
      }
    }
    return specificAnomalies;
  }
};