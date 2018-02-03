#include <dvDetermineNumberOfFiles.h>

#include <cassert>

namespace dv
{

std::size_t
NumberOfSequentialFiles(const std::string&,
                        const std::function<std::size_t(std::string)>&)
{

  std::size_t num_files = 0;

  // TODO: Not implemented.
  assert(false);

  return num_files;

}

}
