#ifndef dv_ReverseFindIf_h
#define dv_ReverseFindIf_h

#include <algorithm>

namespace dv
{
template<typename It, typename Comp>
It ReverseFindIf(It b, It e, Comp c)
{
  const auto r_it = std::find_if(std::reverse_iterator<It>(e),
                                 std::reverse_iterator<It>(b),
                                 c);
  return r_it.base() - 1;
}

}

#endif
