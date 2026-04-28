#pragma once

#include "Strategy.h"

class DummyStrategy : public Strategy {
public:
  explicit DummyStrategy(const TemplatedRequest& blueprint);
  Strategy* clone() const override;

  const std::string& getName() const override;

  const std::string& getDescription() const override;

  std::vector<StrategyResult> run() override;
};