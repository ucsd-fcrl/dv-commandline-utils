#ifndef dvCalculateSurfaceAreas_h
#define dvCalculateSurfaceAreas_h

//#include <vnl/vnl_vector.h>
#include <vector>
#include <vtkPolyData.h>

namespace dv
{

//vnl_vector<double>
std::vector<double>
CalculateSurfaceAreas(vtkPolyData* mesh);

}

#endif
