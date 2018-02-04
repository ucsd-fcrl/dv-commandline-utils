#include <dvInsertionSort.h>
#include <dvMergeSort.h>
#include <dvHeapSort.h>
#include <dvQuickSort.h>
#include <dvHeap.h>
#include <cstdlib>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>

int main()
{

  std::vector<int> v(20);
  std::iota(v.begin(), v.end(), -15);

  ////////////////////
  // Insertion Sort //
  ////////////////////

  std::random_shuffle(v.begin(), v.end());
  dv::InsertionSort(v.begin(), v.end());
  assert(std::is_sorted(v.cbegin(), v.cend()));

  ////////////////
  // Merge Sort //
  ////////////////

  std::random_shuffle(v.begin(), v.end());
  dv::MergeSort(v.begin(), v.end());
  assert(std::is_sorted(v.cbegin(), v.cend()));

  ///////////////
  // Heap Sort //
  ///////////////

  std::random_shuffle(v.begin(), v.end());
  dv::HeapSort(v.begin(), v.end());
  assert(std::is_sorted(v.cbegin(), v.cend()));

  /////////////////////////////////
  // Heap Sort Using Custom Heap //
  /////////////////////////////////

  std::random_shuffle(v.begin(), v.end());
  dv::Heap<int> h;
  for (const auto i : v) h.push(i);
  v.clear();
  while (h.size() > 0) v.push_back(h.pop());
  assert(std::is_sorted(v.cbegin(), v.cend()));

  ////////////////
  // Quick Sort //
  ////////////////

  std::random_shuffle(v.begin(), v.end());
  std::random_shuffle(v.begin(), v.end());
  dv::QuickSort(v.begin(), v.end());
  assert(std::is_sorted(v.cbegin(), v.cend()));

  return EXIT_SUCCESS;

}
