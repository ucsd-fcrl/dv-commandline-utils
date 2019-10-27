#include <cassert>
#include <dvSmartPointer.h>
#include <iostream>

int
main()
{

  using T = dv::SmartPointer<int>;
  T number(new int);

  number.set(4);

  assert(1 == number.get_reference_count());

  {
    T number2(number);

    assert(2 == number.get_reference_count());
    assert(2 == number2.get_reference_count());
  }

  assert(1 == number.get_reference_count());

  T number3 = number;

  assert(2 == number.get_reference_count());

  T number4(5);

  return EXIT_SUCCESS;
}
