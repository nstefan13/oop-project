#pragma once

#include "Strategy.h"

class DummyStrategy : public Strategy {
public:
  DummyStrategy(const TemplatedRequest& blueprint) : Strategy(blueprint) {}
  Strategy* clone() const override { return new DummyStrategy(*this); }

  const std::string& getName() const override {
    static const std::string name = "Dummy";
    return name;
  }

  const std::string& getDescription() const override {
    static const std::string description = "Replaces the payload with the string DUMMY and runs it";
    return description;
  }

  std::vector<StrategyResult> run() override {
    auto request = this->requestBlueprint.compileRequest("DUMMY");
    auto response = request.perform();
    if (response.has_value()) {
      std::cout << "SUCCESS: " << response->get_body().substr(0, 1000);
    } else {
      std::cout << "ERROR: " << response.error();
    }
    return {};
  }
};