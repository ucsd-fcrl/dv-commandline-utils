#ifndef dv_XORSwap_h
#define dv_XORSwap_h

/*
XOR Swap swaps two objects without the use of a temporary variable.
*/

namespace dv
{
template<typename T>
void XORSwap(T &v1, T &v2)
{
  if (&v1 == &v2) return;
  v1 ^= v2; // v1 == A XOR B
  v2 ^= v1; // v2 == B XOR (A XOR B) == A XOR (B XOR B) == A
  v1 ^= v2; // v1 == (A XOR B) XOR A == B XOR (A XOR A) == B
}
}

#endif
