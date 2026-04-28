
#pragma once

#include "../Constants.h"
#include "ResponseDiffEngine.h"
#include "Strategy.h"
#include <format>
#include <iostream>
#include <string>
#include <vector>

class SsjsBooleanBlindStrategy : public Strategy {
public:
  explicit SsjsBooleanBlindStrategy(const TemplatedRequest &requestBlueprint)
      : Strategy(requestBlueprint) {}

  [[nodiscard]] const std::string &getName() const override;

  [[nodiscard]] const std::string &getDescription() const override;

  [[nodiscard]] Strategy *clone() const override;

  std::vector<StrategyResult> run() override;
};
