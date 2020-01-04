#ifndef dv_VTKPolyDataToITKTriangleMesh_h
#define dv_VTKPolyDataToITKTriangleMesh_h

// STD
#include <iostream>

// ITK
#include "itkQuadEdgeMesh.h"
#include "itkLineCell.h"
#include "itkTriangleCell.h"

// VTK
#include "vtkPolyData.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include <vtkCellData.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>

namespace dv
{

template< typename MeshType >
typename MeshType::Pointer
VTKPolyDataToITKTriangleMesh(vtkPolyData* polyData) {

  const auto mesh = MeshType::New();

  // Transfer the points from the vtkPolyData into the itk::Mesh
  const auto numberOfPoints = polyData->GetNumberOfPoints();
  const auto vtkpoints = polyData->GetPoints();
  for (vtkIdType p = 0; p < numberOfPoints; ++p)
    {
    const auto apoint = vtkpoints->GetPoint( p );
    mesh->SetPoint( p, typename MeshType::PointType( apoint ));
    }

  const vtkIdType  * cellPoints;
  vtkIdType    numberOfCellPoints;
  const auto polygons = polyData->GetPolys();
  polygons->InitTraversal();

  // Copy the triangles from vtkPolyData into the itk::Mesh
  using CellType = typename MeshType::CellType;
  using TriangleCellType = typename itk::TriangleCell< CellType >;

  int cellId = 0;

  while( polygons->GetNextCell( numberOfCellPoints, cellPoints ) )
    {
    if( numberOfCellPoints !=3 ) // skip any non-triangle.
      {
      continue;
      }
    typename MeshType::CellAutoPointer c;
    TriangleCellType * t = new TriangleCellType;
    typename TriangleCellType::PointIdentifier itkPts[3];
    for (int ii = 0; ii < numberOfCellPoints; ++ii)
      {
      itkPts[ii] = static_cast<typename TriangleCellType::PointIdentifier>(cellPoints[ii]);
      }
    t->SetPointIds( itkPts );
    c.TakeOwnership( t );
    mesh->SetCell( cellId, c );
    cellId++;
    }

  // Cell Data
  if (nullptr != polyData->GetCellData()->GetScalars()) {
    vtkFloatArray* cell_data = static_cast<vtkFloatArray*>(polyData->GetCellData()->GetScalars());
    for (vtkIdType i = 0; i < cell_data->GetSize(); ++i) {
      mesh->SetCellData( i, cell_data->GetValue(i) );
    }
  }

  // Point Data
  if (nullptr != polyData->GetPointData()->GetScalars()) {
    vtkFloatArray* point_data = static_cast<vtkFloatArray*>(polyData->GetPointData()->GetScalars());
    for (vtkIdType i = 0; i < point_data->GetSize(); ++i) {
      mesh->SetPointData( i, point_data->GetValue(i) );
    }
  }
  mesh->DeleteUnusedCellData();

  return mesh;

}

}

#endif
