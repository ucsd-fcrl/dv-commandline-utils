#ifndef dvJoin_h
#define dvJoin_h

#include <cassert>
#include <iterator>
#include <string>
#include <vector>

namespace dv {

template<typename T>
std::string
to_string(const T& val)
{
  return std::to_string(val);
}

template<>
std::string
to_string(const std::string& val)
{
  return val;
}

template<typename It>
std::string
Join_impl(const std::string& delim,
          const It begin,
          const It end,
          const std::string& append = "")
{

  switch (std::distance(begin, end)) {
    case 1:
      return dv::to_string(*begin) + delim + append;
      break;
    default:
      return Join_impl(
        delim, begin, end - 1, dv::to_string(*(end - 1)) + delim + append);
      break;
  }
}

template<typename It>
std::string
Join(const std::string& delim, const It begin, const It end)
{
  const auto d = std::distance(begin, end);
  assert(d >= 0);
  switch (std::distance(begin, end)) {
    case 0:
      return "";
      break;
    case 1:
      return dv::to_string(*begin);
      break;
    default:
      return Join_impl(delim, begin, end - 1, dv::to_string(*(end - 1)));
  }
}

}

#endif
