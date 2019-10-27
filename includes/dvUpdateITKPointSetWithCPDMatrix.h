#ifndef dv_UpdateITKPointSetWithCPDMatrix_h
#define dv_UpdateITKPointSetWithCPDMatrix_h

#include <cpd/nonrigid.hpp>

namespace dv {
template<typename TPointSet>
typename TPointSet::Pointer
UpdateITKPointSetWithCPDMatrix(typename TPointSet::Pointer points,
                               const cpd::Matrix& mat)
{
  constexpr auto dim = TPointSet::PointType::Dimension;
  using TPSIt = typename TPointSet::PointsContainer::Iterator;

  for (TPSIt it = points->GetPoints()->Begin();
       it != points->GetPoints()->End();
       ++it) {
    for (size_t i = 0; i < dim; ++i) {
      it->Value()[i] = mat(it->Index(), i);
    }
  }

  return points;
}
}

#endif
