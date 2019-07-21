#ifndef dv_SoftMax_h
#define dv_SoftMax_h

#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>

namespace dv
{

template<typename T>
std::vector<T> SoftMax(const std::vector<T> &in)
{
  static_assert(std::is_floating_point<T>::value, "Must be floating point.");
  std::vector<T> p;
  p.resize(in.size());
  const auto e = [](const T element){ return std::exp(element); };
  std::transform(in.cbegin(), in.cend(), p.begin(), e);
  const auto s = std::accumulate(p.cbegin(), p.cend(), static_cast<T>(0.0));
  std::transform(p.cbegin(), p.cend(), p.begin(), [s](const T element){ return element / s; });
  return p;
}

}

#endif
