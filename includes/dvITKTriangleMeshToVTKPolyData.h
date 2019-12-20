#ifndef dv_ITKTriangleMeshToVTKPolyData_h
#define dv_ITKTriangleMeshToVTKPolyData_h

#include <itkMesh.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkTriangle.h>
#include <vtkCellArray.h>

namespace dv {

template<typename TMesh>
vtkSmartPointer<vtkPolyData> ITKTriangleMeshToVTKPolyData(typename TMesh::Pointer mesh) {

  const auto points = vtkSmartPointer<vtkPoints>::New();

  for (auto it = mesh->GetPoints()->Begin();
       it != mesh->GetPoints()->End();
       ++it) {
    points->InsertNextPoint( it.Value()[0], it.Value()[1], it.Value()[2] );
  }

  const auto triangles = vtkSmartPointer<vtkCellArray>::New();
  for (auto it = mesh->GetCells()->Begin();
       it != mesh->GetCells()->End();
       ++it) {
    const auto triangle = vtkSmartPointer<vtkTriangle>::New();
    for (size_t i = 0; i < 3; ++i) {
      triangle->GetPointIds()->SetId(i, it.Value()->GetPointIds()[i]);
    }
    triangles->InsertNextCell(triangle);
  }

  const auto polydata = vtkSmartPointer<vtkPolyData>::New();
  polydata->SetPoints(points);
  polydata->SetPolys(triangles);

  return polydata;
}

}

#endif
