#ifndef dv_HeapSort_h
#define dv_HeapSort_h

#include <algorithm>

/*

Time Efficiency:
- Best, Average, and Worst Case: O(n log(n))

Memory Efficiency:
- Naive: O(N) -- Duplicate data in heap
- Best: O(1) -- In place

Notes:
- Based on the `heap' data structure, which is a complete, binary tree
- Highest priority node is always placed at the root.
- Not stable (doesn't preserve order of equal elements)
*/

namespace dv {

template<typename It>
void
HeapSort(const It b, const It e)
{
  std::make_heap(b, e);
  std::sort_heap(b, e);
}

}

#endif
