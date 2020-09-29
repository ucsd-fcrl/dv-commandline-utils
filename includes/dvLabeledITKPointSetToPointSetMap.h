#ifndef dv_LabeledITKPointSetToPointSetMap_h
#define dv_LabeledITKPointSetToPointSetMap_h

// STD
#include <map>

// ITK
#include <itkPointSet.h>

namespace dv {

template<typename TPointSet>
typename std::map<size_t, typename TPointSet::Pointer>
LabeledITKPointSetToPointSetMap(const typename TPointSet::Pointer points) {

  typename std::map<size_t, typename TPointSet::Pointer> pointset_map;

  for (size_t i = 0; i < points->GetPoints()->Size(); ++i) {
    const auto p = points->GetPoint(i);
    const size_t l = points->GetPointData()->ElementAt(i);
    if (0 == pointset_map.count(l)) {
      pointset_map[l] = TPointSet::New();
    }
    pointset_map[l]->GetPoints()->CastToSTLContainer().push_back(p);
  }

  return pointset_map;
}

}

#endif
