#include "strategies/Strategy.h"

void Strategy::afisare(std::ostream &os) const {
  os << std::format("Strategy {{\n\t name={}\n\t description={}\n}}",
                    this->getName(), this->getDescription());
}

std::ostream &operator<<(std::ostream &os, const Strategy &strategy) {
  strategy.afisare(os);
  return os;
}
