#ifndef dv_Partition_h
#define dv_Partition_h

#include <iterator>
#include <algorithm>

namespace dv
{

/*
- Partitions a range into those that satisfy a predicate and those that do not.
- Return an iterator to the first element of the second range.
- Reimplementation of std::partition (do not use)
*/

template<class It, class UnaryPredicate>
It Partition(const It first, const It last, const UnaryPredicate p)
{
  // Find the first value where the predicate is false.
  auto first_false = std::find_if_not(first, last, p);
  // If no elements cause the predicate to be false, return the first.
  if (first_false == last) return first_false;

  // Loop from (first_false + 1) to last;
  // If the element is true, switch with first and increment.
  for (It i = std::next(first_false); i != last; ++i)
    {
    if (p(*i))
      {
      std::iter_swap(i, first_false);
      ++first_false;
      }
    }
  return first_false;
}

}

#endif
