#ifndef dvCalculateTriangleCenters_h
#define dvCalculateTriangleCenters_h

#include <array>
#include <vector>
#include <vtkPolyData.h>

namespace dv {

std::vector<std::array<double, 3>>
CalculateTriangleCenters(vtkPolyData* mesh);

}

#endif
