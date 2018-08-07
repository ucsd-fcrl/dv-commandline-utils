#ifndef vtkCameraState_h
#define vtkCameraState_h

#include <vtkSmartPointer.h>
#include <vtkCamera.h>

namespace dv
{
class CameraState
{

public:

  double Position[3] = {0.0,0.0,0.0};
  double FocalPoint[3] = {0.0,0.0,0.0};
  double ViewUp[3] = {0.0,0.0,0.0};
  double ViewAngle = 0.0;
  double ParallelScale = 0.0;
  int    ParallelProjection = 0;

  void CaptureState(vtkCamera* const camera);
  void RestoreState(vtkCamera* const camera) const;

}; // end class
} // end namespace vtk
#endif
