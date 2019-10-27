#ifndef dvCalculateSurfaceAreas_h
#define dvCalculateSurfaceAreas_h

#include <vector>
#include <vtkPolyData.h>

namespace dv {

std::vector<double>
CalculateSurfaceAreas(vtkPolyData* mesh);

}

#endif
