#ifndef dv_ITKPointSetToCPDMatrix_h
#define dv_ITKPointSetToCPDMatrix_h

#include <cpd/nonrigid.hpp>

namespace dv {
template<typename TPointSet>
cpd::Matrix
ITKPointSetToCPDMatrix(typename TPointSet::Pointer points)
{
  constexpr auto dim = TPointSet::PointType::Dimension;
  using TPSIt = typename TPointSet::PointsContainer::Iterator;

  cpd::Matrix mat(points->GetNumberOfPoints(), dim);

  for (TPSIt it = points->GetPoints()->Begin();
       it != points->GetPoints()->End();
       ++it) {
    for (size_t i = 0; i < dim; ++i) {
      mat(it->Index(), i) = it->Value()[i];
    }
  }

  return mat;
}

}

#endif
