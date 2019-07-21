#include <dvXORSwap.h>
#include <cstdlib>
#include <cassert>

int main()
{

  int v1 = 42;
  int v2 = 65;

  dv::XORSwap(v1, v2);

  assert( (65 == v1) && (42 == v2) );

  dv::XORSwap(v1, v1);

  assert( 65 == v1 );

  return EXIT_SUCCESS;
}
