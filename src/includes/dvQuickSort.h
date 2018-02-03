#ifndef dv_QuickSort_h
#define dv_QuickSort_h

#include <algorithm>
#include <dvPartition.h>

/*
Algorithm:

- Pick an element, called a pivot, from the array.
- Partitioning such that all elements < pivot come before and all elements > pivot come after.
- Recursively apply the above steps to the sub-array of elements with smaller values and separately to the sub-array of elements with greater values (neither subarray includes pivot).


Time Efficiency:
- Worst Case: O(n^2) -- Occurs if the worst pivot possible is always chosen.
- Best Case: O(n log(n)) -- Occurs if the pivot is always chosen to be the median.
- Average Case: O(n log(n)) -- Occurs if the sequence is sufficiently random.

Notes:
- The worst case really matters in this case, as semi-sorted sequences are very common in practice.
*/

namespace dv
{

template<typename It>
void QuickSort(const It b, const It e)
{

  if (b == e) return;

  // Get some pivot.
  const auto pivot = *std::next(b, std::distance(b, e) / 2);

  using T = typename It::value_type;

  const auto func1 = [pivot](const T &element){ return element<pivot; };
  const auto func2 = [pivot](const T &element){ return element==pivot; };

  const auto m1 = dv::Partition(b, e, func1);
  const auto m2 = dv::Partition(m1, e, func2);
//  const auto m1 = std::partition(b, e, func1);
//  const auto m2 = std::partition(m1, e, func2);

  QuickSort(b, m1);
  QuickSort(m2, e);

}

}

#endif
