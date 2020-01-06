#ifndef dv_RefineValenceThreeVertices_h
#define dv_RefineValenceThreeVertices_h

#include <itkQuadEdgeMesh.h>
#include <itkQuadEdgeMeshEulerOperatorDeleteCenterVertexFunction.h>

namespace dv {

template<typename TMesh>
void
RefineValenceThreeVertices(typename TMesh::Pointer mesh) {

  using TDeleteVertex = itk::QuadEdgeMeshEulerOperatorDeleteCenterVertexFunction< TMesh, typename TMesh::QEType >;

  bool vertex_was_deleted = false;
  do {
    vertex_was_deleted = false;
    for (auto it = mesh->GetPoints()->Begin(); it != mesh->GetPoints()->End(); ++it) {
      if (3 != it.Value().GetValence()) {
        continue;
      }
      auto qe = mesh->FindEdge( it.Index() );
      if (3 != mesh->GetPoint(qe->GetDestination()).GetValence()) {
        qe = qe->GetSym();
      }



      std::vector<typename TMesh::CellIdentifier> deleted_cell_ids;
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

}

}

#endif
