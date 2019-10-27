#ifndef dvGetVTKTransformationMatrixFromITKImage_h
#define dvGetVTKTransformationMatrixFromITKImage_h

namespace dv {

/*
 Given an ITK image, use the origin and direction to create a vtkMatrix.
 This matrix can be used to construct a vtkTransform, which will take data in
 image coordinates and transform them into world coordinates.
 */
template<typename TImage>
void
GetVTKTransformationMatrixFromITKImage(const typename TImage::Pointer image,
                                       vtkMatrix4x4* mat)
{
  // Direction Matrix
  typename TImage::DirectionType d = image->GetDirection();
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      mat->SetElement(i, j, d(i, j));
    }
  }

  // Origin
  typename TImage::PointType o = image->GetOrigin();
  for (int i = 0; i < 3; ++i) {
    mat->SetElement(i, 3, o[i]);
  }
}

}

#endif
