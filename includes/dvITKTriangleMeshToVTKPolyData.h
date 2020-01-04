#ifndef dv_ITKTriangleMeshToVTKPolyData_h
#define dv_ITKTriangleMeshToVTKPolyData_h

#include <itkMesh.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkCellData.h>
#include <vtkPoints.h>
#include <vtkTriangle.h>
#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>

namespace dv {

template<typename TMesh>
vtkSmartPointer<vtkPolyData> ITKTriangleMeshToVTKPolyData(typename TMesh::Pointer mesh) {

  const auto polydata = vtkSmartPointer<vtkPolyData>::New();

  // Points
  if (nullptr != mesh->GetPoints() && (mesh->GetNumberOfPoints() > 0)) {
    const auto points = vtkSmartPointer<vtkPoints>::New();
    for (auto it = mesh->GetPoints()->Begin();
         it != mesh->GetPoints()->End();
         ++it) {
      points->InsertPoint(
        it.Index(),
        it.Value()[0],
        it.Value()[1],
        it.Value()[2] );
    }
    polydata->SetPoints(points);
  }

  // Point Data
  if (nullptr != mesh->GetPointData() &&
      (mesh->GetPointData()->Size() > 0)) {
    const auto point_data = vtkSmartPointer<vtkFloatArray>::New();
    for (auto it = mesh->GetPointData()->Begin();
         it != mesh->GetPointData()->End();
         ++it) {
      point_data->InsertValue(it.Index(), it.Value());    
    }
    polydata->GetPointData()->SetScalars(point_data);
  }

  // Cells
  if (nullptr != mesh->GetCells() && (mesh->GetNumberOfCells() > 0)) {
    const auto triangles = vtkSmartPointer<vtkCellArray>::New();
    const auto cell_data = vtkSmartPointer<vtkFloatArray>::New();
    for (auto it = mesh->GetCells()->Begin();
         it != mesh->GetCells()->End();
         ++it) {
      const auto triangle = vtkSmartPointer<vtkTriangle>::New();
      for (size_t i = 0; i < 3; ++i) {
        triangle->GetPointIds()->SetId(i, it.Value()->GetPointIds()[i]);
      }
      triangles->InsertNextCell( triangle );
      // Cell Data
      if (nullptr != mesh->GetCellData() && (mesh->GetCellData()->Size() > 0)) {
        if (mesh->GetCellData()->IndexExists(it.Index())) {
          cell_data->InsertNextValue(
            mesh->GetCellData()->ElementAt(it.Index()));
        } else {
          cell_data->InsertNextValue( 0 );
        }
      }
    }
    if (nullptr != mesh->GetCellData() && (mesh->GetCellData()->Size() > 0)) {
      polydata->GetCellData()->SetScalars(cell_data);
    }
    polydata->SetPolys(triangles);
  }

  return polydata;
}

}

#endif
