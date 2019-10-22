#ifndef dv_MergeSort_h
#define dv_MergeSort_h

#include <algorithm>

/*
- O(nlog(n)) sorting algorithm
- Used in Timsort
*/
namespace dv
{

template<typename It>
void MergeSort(const It b, const It e)
{
  const auto n = std::distance(b, e);
  if (n < 2) return;
  const auto m = std::next(b, n / 2);
  MergeSort(b, m);
  MergeSort(m, e);
  std::inplace_merge(b, m, e);
}

}

#endif
