// System
#include <cassert>

// Third Party
#include <boost/filesystem.hpp>

// Custom
#include <dvNumberOfSequentialFiles.h>

namespace dv
{

size_t
NumberOfSequentialFiles(const std::function<std::string(size_t)> formatter)
{

  size_t num_files = 0;

  while (boost::filesystem::exists(formatter(num_files))) ++num_files;

  return num_files;

}

}
