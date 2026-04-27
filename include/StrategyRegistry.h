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
  static StrategyRegistry &getInstance() {
    if (!instance) {
      instance = new StrategyRegistry();
    }
    return *instance;
  }

  //
  // REAL FUNCTIONS
  //
  void addStrategy(std::unique_ptr<Strategy> strategy) {
    strategies.push_back(std::move(strategy));
  }

  std::vector<StrategyResult> runAll() {
    std::vector<StrategyResult> allAnomalies;
    for (const auto &strat : strategies) {
      std::cout << "[*] Running strategy: " << strat->getName() << "\n";
      auto results = strat->run();
      allAnomalies.insert(allAnomalies.end(), results.begin(), results.end());
    }
    return allAnomalies;
  }

  template <typename SpecificStrategyType>
  std::vector<StrategyResult> runOnly() {
    std::vector<StrategyResult> specificAnomalies;
    for (const auto &strat : strategies) {
      // Downcast cu sens: verificam daca pointerul de baza pointeaza catre
      // derivata cautata
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

StrategyRegistry *StrategyRegistry::instance = nullptr;