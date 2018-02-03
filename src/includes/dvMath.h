#ifndef dvMath_h
#define dvMath_h

#include <type_traits>
#include <cmath>
#include <algorithm>

namespace dv
{

template<typename T>
bool
Close(const T& v1, const T& v2, const T& epsilon = 10e-6)
{
  static_assert(std::is_floating_point<T>::value, "Error: Expected floating point input.");
  return (std::fabs(v1 - v2) < epsilon);
}

template<typename T>
T
Clamp(const T& input, const T& lower, const T& upper)
{
  return std::max(std::min(input, upper), lower);
}

}

#endif
