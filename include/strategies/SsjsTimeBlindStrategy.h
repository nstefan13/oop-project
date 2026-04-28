
#pragma once

#include "../Constants.h"
#include "Strategy.h"
#include <format>
#include <iostream>
#include <string>
#include <vector>

class SsjsTimeBlindStrategy : public Strategy {
public:
  explicit SsjsTimeBlindStrategy(const TemplatedRequest &requestBlueprint)
      : Strategy(requestBlueprint) {}

  [[nodiscard]] const std::string &getName() const override;

  [[nodiscard]] const std::string &getDescription() const override;

  [[nodiscard]] Strategy *clone() const override;

  std::vector<StrategyResult> run() override;
};
