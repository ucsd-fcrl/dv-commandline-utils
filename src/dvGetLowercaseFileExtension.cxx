#include <dvGetLowercaseFileExtension.h>

namespace dv {

std::string
GetLowercaseFileExtension(const std::string& fileName)
{
  auto ext = fs::extension(fs::path(fileName));
  std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) {
    return std::tolower(c);
  });
  return ext;
}

}
