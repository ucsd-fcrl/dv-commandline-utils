#ifndef dv_DetermineNumberOfFiles_h
#define dv_DetermineNumberOfFiles_h

#include <cstdlib>
#include <functional>

namespace
dv
{

std::size_t
NumberOfSequentialFiles(const std::string& directory,
                        const std::function<std::size_t(std::string)>& formatter);

}

#endif
