#pragma once

#include <functional>
#include <utility>
#include <vector>

template <typename T> class Pipeline {
  using PipelineStep = std::function<T(T)>;
  std::vector<PipelineStep> steps;

public:
  void addStep(PipelineStep step) { steps.push_back(step); }

  T apply(T raw) {
    T res = std::move(raw);
    for (auto &&step : steps) {
      res = step(std::move(res));
    }
    return res;
  }
};