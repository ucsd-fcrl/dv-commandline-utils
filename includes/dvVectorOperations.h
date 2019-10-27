#ifndef dv_VectorOperations_h
#define dv_VectorOperations_h

#include <algorithm>
#include <stdexcept>
#include <vector>

namespace dv {

template<typename T>
std::vector<bool>
ElementwiseEqual(const std::vector<T>& input, const T& value)
{
  std::vector<bool> output(input.size());
  const auto lambda = [value](T i) -> bool { return value == i; };
  std::transform(input.cbegin(), input.cend(), output.begin(), lambda);
  return output;
}

template<typename T>
std::vector<T>
BooleanIndexing(const std::vector<T>& input, const std::vector<bool> logic)
{
  if (input.size() != logic.size()) {
    throw std::length_error("Input and index vectors must be the same length.");
  }

  std::vector<T> output;
  for (size_t i = 0; i < input.size(); ++i) {
    if (logic.at(i))
      output.emplace_back(input[i]);
  }
  return output;
}

}

#endif
