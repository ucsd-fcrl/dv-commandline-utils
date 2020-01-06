// ITK
#include <itkTriangleHelper.h>

namespace dv {

template<typename TMesh>
void
AssignCellDataByOctant(typename TMesh::Pointer mesh) {

  using TTriangleHelper = itk::TriangleHelper< typename TMesh::PointType >;

  // Assign cell data (different for each octant).
  for (auto it = mesh->GetCells()->Begin();
    it != mesh->GetCells()->End();
    ++it) {
    const auto cell = it.Value();

    const auto centroid = TTriangleHelper::ComputeGravityCenter(
      mesh->GetPoint(cell->GetPointIds()[0]),
      mesh->GetPoint(cell->GetPointIds()[1]),
      mesh->GetPoint(cell->GetPointIds()[2])
    );

    unsigned int data = 0;
    if (centroid[0] < 0 && centroid[1] < 0 && centroid[2] < 0) {
      data = 1;
    } else if (centroid[0] < 0 && centroid[1] < 0 && centroid[2] >= 0) {
      data = 2;
    } else if (centroid[0] < 0 && centroid[1] >= 0 && centroid[2] < 0) {
      data = 3;
    } else if (centroid[0] < 0 && centroid[1] >= 0 && centroid[2] >= 0) {
      data = 4;
    } else if (centroid[0] >= 0 && centroid[1] < 0 && centroid[2] < 0) {
      data = 5;
    } else if (centroid[0] >= 0 && centroid[1] < 0 && centroid[2] >= 0) {
      data = 6;
    } else if (centroid[0] >= 0 && centroid[1] >= 0 && centroid[2] < 0) {
      data = 7;
    } else {
      data = 8;
    }
    itkAssertOrThrowMacro(data != 0, "Not possible.");
    mesh->SetCellData( it.Index(), data );
  }

}

}
