#pragma once

#include "TemplatedRequest.h"
#include <format>
#include <iostream>
#include <string>

#include <vector>
#include "StrategyResult.h"

class Strategy {
protected:
  TemplatedRequest requestBlueprint;

public:
  explicit Strategy(const TemplatedRequest &_requestBlueprint)
      : requestBlueprint{_requestBlueprint} {}
  Strategy() = delete;
  // OBLIGATORIU
  virtual ~Strategy() = default;

  [[nodiscard]] virtual const std::string &getName() const = 0;
  [[nodiscard]] virtual const std::string &getDescription() const = 0;
  virtual std::vector<StrategyResult> run() = 0;

  [[nodiscard]] virtual Strategy *clone() const = 0;

  virtual void afisare(std::ostream &os) const;
  friend std::ostream &operator<<(std::ostream &os, const Strategy &strategy);
};