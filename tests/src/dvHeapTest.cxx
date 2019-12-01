#include <cstdlib>
#include <dvHeap.h>
#include <iostream>

int
main()
{
  using THeap = dv::Heap<int>;

  THeap h;
  for (size_t i = 0; i < 20; ++i) {
    h.push(i * 100 % 7);
    for (const auto d : h.data)
      std::cout << d << ' ';
    std::cout << std::endl;
  }

  while (h.size() > 0) {
    h.pop();
    for (const auto d : h.data)
      std::cout << d << ' ';
    std::cout << std::endl;
  }

  return EXIT_SUCCESS;
}
