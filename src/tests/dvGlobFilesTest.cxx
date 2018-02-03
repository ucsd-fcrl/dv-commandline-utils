#include <dvGlobFiles.h>
#include <cstdlib>
#include <iostream>
int
main()
{

  const auto files = dv::GlobFiles("~/Desktop/*");

  for (const auto &f : files) std::cout << f << std::endl;

  return EXIT_SUCCESS;
}

