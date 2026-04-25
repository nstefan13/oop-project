#pragma once

#include <iostream>
#include <string>
#include <format>
#include "TemplatedRequest.h"

class Strategy {
protected:
  TemplatedRequest requestBlueprint;

public:
  Strategy(const TemplatedRequest& _requestBlueprint) : requestBlueprint{_requestBlueprint} {}
  Strategy() = delete;
  virtual ~Strategy() = default;

  virtual const std::string& getName() const = 0;
  virtual const std::string& getDescription() const = 0;
  virtual void run() = 0;

  virtual Strategy* clone() const = 0;

  virtual void afisare(std::ostream& os) const {
    os << std::format("Strategy {{\n\t name={}\n\t description={}\n}}", this->getName(), this->getDescription());
  }
  friend std::ostream& operator<<(std::ostream& os, const Strategy& strategy) {
    strategy.afisare(os);
    return os;
  }

  // OBLIGATORIU
  virtual Strategy::~Strategy() = default;
};