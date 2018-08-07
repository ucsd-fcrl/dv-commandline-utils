#include <dvCalculateSurfaceAreas.h>
#include <vtkTriangle.h>

namespace dv
{

std::vector<double>
CalculateSurfaceAreas(vtkPolyData* mesh)
{

  std::vector<double> out(mesh->GetNumberOfCells(), -1);

  double p0[3], p1[3], p2[3];
  double area;
  for (vtkIdType i = 0; i < mesh->GetNumberOfCells(); ++i)
    {
    vtkCell* cell = mesh->GetCell(i);
    vtkTriangle* triangle = dynamic_cast<vtkTriangle*>(cell);
    triangle->GetPoints()->GetPoint(0, p0);
    triangle->GetPoints()->GetPoint(1, p1);
    triangle->GetPoints()->GetPoint(2, p2);
    area = vtkTriangle::TriangleArea(p0, p1, p2);
    if (area < 0)
      {
      std::cerr << "Warning:\nArea is less than zero (" << area << ')'
                << "Point 0: " << p0[0] << ' ' << p0[1] << ' ' << p0[2]
                << "Point 1: " << p1[0] << ' ' << p1[1] << ' ' << p1[2]
                << "Point 2: " << p2[0] << ' ' << p2[1] << ' ' << p2[2]
                << std::endl;
      }
    out[i] = area;
    }

  return out;

}

}
