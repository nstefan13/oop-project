#include "StrategyRegistry.h"

StrategyRegistry *StrategyRegistry::instance = nullptr;

StrategyRegistry &StrategyRegistry::getInstance() {
  if (!instance) {
    instance = new StrategyRegistry();
  }
  return *instance;
}

void StrategyRegistry::addStrategy(std::unique_ptr<Strategy> strategy) {
  strategies.push_back(std::move(strategy));
}

std::vector<StrategyResult> StrategyRegistry::runAll() {
  std::vector<StrategyResult> allAnomalies;
  for (const auto &strat : strategies) {
    std::cout << "[*] Running strategy: " << strat->getName() << "\n";
    auto results = strat->run();
    allAnomalies.insert(allAnomalies.end(), results.begin(), results.end());
  }
  return allAnomalies;
}
