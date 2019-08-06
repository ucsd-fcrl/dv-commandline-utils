#ifndef dv_MeshToQuadEdgeMesh_h
#define dv_MeshToQuadEdgeMesh_h

#include <itkMesh.h>
#include <itkQuadEdgeMesh.h>

namespace dv
{

template
<typename TMesh, typename TQEMesh>
void
MeshToQuadEdgeMesh(TMesh *input, TQEMesh *output)
{
  // Points
  for (auto it = input->GetPoints()->Begin();
       it != input->GetPoints()->End();
       ++it)
    {
    output->SetPoint( it->Index(), it->Value() );
    }

  // Point Data
  for (auto it = input->GetPointData()->Begin();
       it != input->GetPointData()->End();
       ++it)
    {
    output->SetPointData( it->Index(), it->Value() );
    }

  // Cells
  for (auto it = input->GetCells()->Begin();
       it != input->GetCells()->End();
       ++it)
    {
    const auto N = it->Value()->GetNumberOfPoints();
    std::vector<size_t> pt_ids;
    for (size_t i = 0; i < N; ++i)
      {
      pt_ids.push_back(it->Value()->GetPointIds()[i]);
      }
    output->AddFace( pt_ids );
    }

  // Cell Data
  for (auto it = input->GetCellData()->Begin();
       it != input->GetCellData()->End();
       ++it)
    {
    output->SetPointData( it->Index(), it->Value() );
    }
}

}

#endif
