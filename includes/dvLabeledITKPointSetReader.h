#ifndef dv_LabeledITKPointSetReader_h
#define dv_LabeledITKPointSetReader_h

#include <iostream>
#include <fstream>

namespace dv {
template<typename TPointSet>
typename TPointSet::Pointer
LabeledITKPointSetReader(const std::string&file_name) {

  const auto points = TPointSet::New();

  std::ifstream file(file_name);

  typename TPointSet::PointType point;
  unsigned char l;
  size_t i = 0;

  while (file >> point[0] >> point[1] >> point[2] >> l) {
    points->SetPoint(i, point);
    points->SetPointData(i, l);
    ++i;
  }

  file.close();

  return points;

}
}

#endif
