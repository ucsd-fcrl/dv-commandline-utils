#ifndef dv_NumberOfSequentialFiles_h
#define dv_NumberOfSequentialFiles_h

#include <cstdlib>
#include <functional>

namespace
dv
{

size_t
NumberOfSequentialFiles(const std::function<std::string(size_t)> formatter);

}

#endif
