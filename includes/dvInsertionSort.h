#ifndef dv_InsertionSort_h
#define dv_InsertionSort_h

#include <algorithm>
#include <dvReverseFindIf.h>
#include <iostream>
//#include <vector>

/*
dv::InsertionSort implements a simple sorting algorithm.

Algorithm:
- Array position 0 is trivially sorted.
- Begin at array position i = 1.
- Move backward until you find an element j that is larger than i.
- Shift the elemts from j to i - 1 forward one spot.
- Put i in position j.

Time Efficiency:
- Best Case: O(N) -- already sorted.
- Worst Case: O(N^2) -- reverse sorted.
- Average Case: O(N^2)

Memory Efficiency:
- O(1) -- Sorting occurs in place.

Other Comments:
- Simple to implement
- Can be efficient for small and nearly sorted data sets
- Can be used as a subroutine in a more sophisticated algorithm (i.e., Tim Sort)
*/

namespace dv {

template<typename It>
void
InsertionSort(const It b, const It e)
{

  for (auto k = b + 1; k != e; ++k) {
    const auto comp = [k](const typename It::value_type& element) {
      return element <= (*k);
    };
    // Don't actually want to find the last that is <=, but the first that is >.
    // For this reason we need to add 1.
    const auto it = dv::ReverseFindIf(b, k, comp) + 1;
    if (it == k)
      continue;
    std::rotate(it, k, k + 1);
  }
}

}

#endif
