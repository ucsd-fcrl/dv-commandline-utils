#ifndef dv_GetLowercaseFileExtension_h
#define dv_GetLowercaseFileExtension_h

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

namespace dv
{

std::string
GetLowercaseFileExtension(const std::string &fileName);

}

#endif
