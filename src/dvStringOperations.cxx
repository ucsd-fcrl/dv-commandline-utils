#include <dvStringOperations.h>

namespace dv {

std::string
TrimCharactersFromEnd(const std::string& s, const char c)
{
  std::string r(s);
  while (c == r.back())
    r.pop_back();
  return r;
}

std::string
AppendCharacterIfAbsent(const std::string& s, char c)
{
  std::string r(s);
  if (c != r.back())
    r.push_back(c);
  return r;
}

}
