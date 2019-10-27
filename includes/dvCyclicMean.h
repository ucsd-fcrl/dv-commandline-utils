#ifndef dv_CyclicMean_h
#define dv_CyclicMean_h

#include <cassert>

namespace dv {

template<typename T>
T
CyclicMean(const T& val1, const T& val2, const T& l_bound, const T& u_bound)
{
  assert(l_bound < u_bound);
  assert((l_bound <= val1) && (val1 < u_bound));
  assert((l_bound <= val2) && (val2 < u_bound));
  const T range = u_bound - l_bound;
  const T val2_norm = (val1 < val2) ? val2 : val2 + range;
  const T avg = (val1 + val2_norm) / 2;
  return (avg < u_bound) ? avg : avg - range;
}

}

#endif
