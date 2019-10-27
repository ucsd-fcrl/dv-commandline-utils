#include <dvCalculateTriangleCenters.h>
#include <vtkTriangle.h>

namespace dv {

std::vector<std::array<double, 3>>
CalculateTriangleCenters(vtkPolyData* mesh)
{

  std::vector<std::array<double, 3>> out;
  out.resize(mesh->GetNumberOfCells());

  double t0[3], t1[3], t2[3], tc[3];

  for (vtkIdType i = 0; i < mesh->GetNumberOfCells(); ++i) {
    vtkCell* cell = mesh->GetCell(i);
    vtkTriangle* triangle = dynamic_cast<vtkTriangle*>(cell);
    triangle->GetPoints()->GetPoint(0, t0);
    triangle->GetPoints()->GetPoint(1, t1);
    triangle->GetPoints()->GetPoint(2, t2);
    vtkTriangle::TriangleCenter(t0, t1, t2, tc);
    out[i][0] = tc[0];
    out[i][1] = tc[1];
    out[i][2] = tc[2];
  }

  return out;
}

}
