#ifndef dv_DeleteIsolatedPoints_h
#define dv_DeleteIsolatedPoints_h

#include <algorithm>
#include <set>

namespace dv {
template<typename MeshType>
void
DeleteIsolatedPoints(MeshType* mesh)
{

  // Get a list of all points which are linked to cells in the mesh.

  mesh->BuildCellLinks();
  using TCellID = typename MeshType::CellIdentifier;
  std::set<TCellID> LinkedPoints;
  const auto LinkedPointsLambda = [](const auto it) { return it.Index(); };

  std::transform(mesh->GetCellLinks()->Begin(),
                 mesh->GetCellLinks()->End(),
                 std::inserter(LinkedPoints, LinkedPoints.begin()),
                 LinkedPointsLambda);

  // Get those points which are NOT linked to any cells in the mesh.

  using TPointID = typename MeshType::PointIdentifier;
  std::set<TPointID> IsolatedPoints;
  for (auto p_it = mesh->GetPoints()->Begin(); p_it != mesh->GetPoints()->End();
       ++p_it) {
    if (0 == LinkedPoints.count(p_it.Index())) {
      IsolatedPoints.emplace(p_it.Index());
    }
  }

  // Delete those points, and their associated point data.

  for (const auto u : IsolatedPoints) {
    mesh->GetPoints()->DeleteIndex(u);
    mesh->GetPointData()->DeleteIndex(u);
  }
}
}

#endif
