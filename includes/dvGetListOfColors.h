#ifndef dv_GetListOfColors_h
#define dv_GetListOfColors_h

#include <array>
#include <vector>

namespace dv
{

std::vector<std::array<double, 3>>
GetListOfColors()
{

  std::vector<std::array<double, 3>> colors;
  colors.emplace_back(std::array<double, 3>{ 0.0, 0.0, 0.0 });
  colors.emplace_back(
    std::array<double, 3>{ 255.0, 0.0, 0.0 });
  colors.emplace_back(
    std::array<double, 3>{ 255.0, 255.0, 0.0 });
  colors.emplace_back(
    std::array<double, 3>{ 127.0, 255.0, 0.0 });
  colors.emplace_back(
    std::array<double, 3>{ 255.0, 0.0, 255.0 });
  colors.emplace_back(
    std::array<double, 3>{ 255.0, 127.0, 0.0 });
  colors.emplace_back(
    std::array<double, 3>{ 0.0, 127.0, 255.0 });
  colors.emplace_back(
    std::array<double, 3>{ 255.0, 0.0, 127.0 });
  colors.emplace_back(
    std::array<double, 3>{ 0.0, 27.0, 155.0 });
  colors.emplace_back(
    std::array<double, 3>{ 155.0, 0.0, 27.0 });
  colors.emplace_back(
    std::array<double, 3>{ 0.0, 255.0, 0.0 });
  colors.emplace_back(
    std::array<double, 3>{ 0.0, 255.0, 255.0 });
  colors.emplace_back(
    std::array<double, 3>{ 0.0, 255.0, 127.0 });
  colors.emplace_back(
    std::array<double, 3>{ 127.0, 0.0, 255.0 });
  colors.emplace_back(
    std::array<double, 3>{ 0.0, 0.0, 255.0 });
  colors.emplace_back(
    std::array<double, 3>{ 255.0, 127.0, 255.0 });
  colors.emplace_back(
    std::array<double, 3>{ 127.0, 127.0, 255.0 });
  colors.emplace_back(
    std::array<double, 3>{ 127.0, 127.0, 127.0 });

  for (auto &color : colors)
    {
    for (auto &channel : color)
      {
      channel /= 255;
      }
    }

  return colors;
}

}

#endif
