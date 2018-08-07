#ifndef dvCalculateTriangleCenters_h
#define dvCalculateTriangleCenters_h

#include <vector>
#include <array>
#include <vtkPolyData.h>

namespace dv
{

std::vector<std::array<double,3>>
CalculateTriangleCenters(vtkPolyData* mesh);

}

#endif
