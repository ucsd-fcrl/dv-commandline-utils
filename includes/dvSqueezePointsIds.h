#ifndef dv_SqueezePointsIds_h
#define dv_SqueezePointsIds_h

#include <map>

namespace dv {
template<typename TMesh>
void
SqueezePointsIds(TMesh* mesh)
{

  // Maps current index to squeezed index
  std::map<unsigned int, unsigned int> v_map;

  unsigned int index = 0;
  for (auto it = mesh->GetPoints()->Begin(); it != mesh->GetPoints()->End();
       ++it) {
    v_map[it->Index()] = index;
    ++index;
  }

  // Create a new point container
  const auto points = TMesh::PointsContainer::New();

  for (auto it = mesh->GetPoints()->Begin(); it != mesh->GetPoints()->End();
       ++it) {
    points->InsertElement(v_map[it->Index()], it->Value());
  }

  mesh->SetPoints(points);

  // Create a new point data container
  const auto point_data = TMesh::PointDataContainer::New();

  for (auto it = mesh->GetPointData()->Begin();
       it != mesh->GetPointData()->End();
       ++it) {
    point_data->InsertElement(v_map[it->Index()], it->Value());
  }

  mesh->SetPointData(point_data);

  // Re-map the cells
  for (auto it = mesh->GetCells()->Begin(); it != mesh->GetCells()->End();
       ++it) {
    const auto N = it->Value()->GetNumberOfPoints();
    for (size_t i = 0; i < N; ++i) {
      const auto current_id = it->Value()->GetPointIds()[i];
      it->Value()->SetPointId(i, v_map[current_id]);
    }
  }
}
}

#endif
