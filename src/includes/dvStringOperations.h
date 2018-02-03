#ifndef dv_StringOperations_h
#define dv_StringOperations_h

#include <string>

namespace dv
{

std::string
TrimCharactersFromEnd(const std::string& s, char c);

std::string
AppendCharacterIfAbsent(const std::string& s, char c);

}

#endif
