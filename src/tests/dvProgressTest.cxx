
#include <dvProgress.h>

int
main(int, char**)
{

  std::size_t N = 25;
  auto pgrs = dv::Progress(N);
  for (std::size_t i = 0; i < N; ++i)
    {
    pgrs.UnitCompleted();
    }

  return EXIT_SUCCESS;
}
