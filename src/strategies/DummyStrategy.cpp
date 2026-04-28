#include "strategies/DummyStrategy.h"

DummyStrategy::DummyStrategy(const TemplatedRequest& blueprint) : Strategy(blueprint) {}

Strategy* DummyStrategy::clone() const { return new DummyStrategy(*this); }

const std::string& DummyStrategy::getName() const {
  static const std::string name = "Dummy";
  return name;
}

const std::string& DummyStrategy::getDescription() const {
  static const std::string description = "Replaces the payload with the string DUMMY and runs it";
  return description;
}

std::vector<StrategyResult> DummyStrategy::run() {
  auto request = this->requestBlueprint.compileRequest("DUMMY");
  auto response = request.perform();
  if (response.has_value()) {
    std::cout << "SUCCESS: " << response->get_body().substr(0, 1000);
  } else {
    std::cout << "ERROR: " << response.error();
  }
  return {};
}
