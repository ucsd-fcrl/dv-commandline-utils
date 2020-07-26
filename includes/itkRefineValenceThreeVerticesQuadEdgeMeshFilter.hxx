/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkRefineValenceThreeVerticesQuadEdgeMeshFilter_hxx
#define itkRefineValenceThreeVerticesQuadEdgeMeshFilter_hxx

#include "itkRefineValenceThreeVerticesQuadEdgeMeshFilter.h"
#include <itkQuadEdgeMeshEulerOperatorDeleteCenterVertexFunction.h>

#include <itkMeshFileWriter.h>

namespace itk
{

template <typename TInputMesh, typename TOutputMesh>
RefineValenceThreeVerticesQuadEdgeMeshFilter<TInputMesh, TOutputMesh>::RefineValenceThreeVerticesQuadEdgeMeshFilter()
{
}

template <typename TInputMesh, typename TOutputMesh>
void
RefineValenceThreeVerticesQuadEdgeMeshFilter<TInputMesh, TOutputMesh>::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template <typename TInputMesh, typename TOutputMesh>
void
RefineValenceThreeVerticesQuadEdgeMeshFilter<TInputMesh, TOutputMesh>::GenerateData()
{

  this->CopyInputMeshToOutputMesh();

  typename TOutputMesh::Pointer     mesh = this->GetOutput();

  using TDeleteVertex = itk::QuadEdgeMeshEulerOperatorDeleteCenterVertexFunction< TOutputMesh, typename TOutputMesh::QEType >;

  bool vertex_was_deleted = false;

  do {
    vertex_was_deleted = false;
    for (auto it = mesh->GetPoints()->Begin(); it != mesh->GetPoints()->End(); ++it) {
      if (3 != it.Value().GetValence()) {
        continue;
      }

      auto qe = mesh->FindEdge( it.Index() );
      if (nullptr == qe) {
        continue;
      }
      if (3 != mesh->GetPoint(qe->GetDestination()).GetValence()) {
        qe = qe->GetSym();
      }

      // Will need list of deleted cell IDs in order to delete cell data later.
      std::vector<typename TOutputMesh::CellIdentifier> deleted_cell_ids;
      auto temp = qe->GetSym();
      do
        {
        temp = temp->GetOnext();
        deleted_cell_ids.push_back( temp->GetLeft() );
        } while (temp != qe->GetSym()); 




      itkAssertOrThrowMacro(3 == mesh->GetPoint(qe->GetDestination()).GetValence(),
        "Origin valence must equal 3.");
      const auto delete_vertex = TDeleteVertex::New();
      delete_vertex->SetInput( mesh );
      const auto new_qe = delete_vertex->Evaluate( qe );
      mesh->DeletePoint(delete_vertex->GetOldPointID());

      if (nullptr != mesh->GetCellData()) {
        const auto cell_data = mesh->GetCellData()->ElementAt(deleted_cell_ids.front());
        for (const auto &id : deleted_cell_ids) {
          mesh->GetCellData()->DeleteIndex(id);
        }
        mesh->SetCellData( new_qe->GetLeft(), cell_data );
      }

      vertex_was_deleted = true;
      break;
    }
  } while (vertex_was_deleted);

  mesh->SqueezePointsIds();

}
} // end namespace itk

#endif
