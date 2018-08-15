#include <vtkActor.h>
#include <vtkArrowSource.h>
#include <vtkMath.h>
#include <vtkMinimalStandardRandomSequence.h>
#include <vtkNamedColors.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkOBJWriter.h>
#include <vtkOBJReader.h>
#include <vtkAxesActor.h>

#include <array>

namespace dv
{
void ArrowWithStartAndEnd(const double startPoint[3],
                     const double endPoint[3],
                     const std::string fileName,
                     const int tipResolution,
                     const int shaftResolution)
{

  const auto arrowSource = vtkSmartPointer<vtkArrowSource>::New();
  arrowSource->SetTipResolution( tipResolution );
  arrowSource->SetShaftResolution( shaftResolution );

  const auto rng = vtkSmartPointer<vtkMinimalStandardRandomSequence>::New();
  rng->SetSeed(8775070);

  // Compute a basis
  double normalizedX[3];
  double normalizedY[3];
  double normalizedZ[3];

  // The X axis is a vector from start to end
  vtkMath::Subtract(endPoint, startPoint, normalizedX);
  double length = vtkMath::Norm(normalizedX);
  vtkMath::Normalize(normalizedX);

  // The Z axis is an arbitrary vector cross X
  double arbitrary[3];
  for (auto i = 0; i < 3; ++i)
    {
    rng->Next();
    arbitrary[i] = rng->GetRangeValue(-10, 10);
    }
  vtkMath::Cross(normalizedX, arbitrary, normalizedZ);
  vtkMath::Normalize(normalizedZ);

  // The Y axis is Z cross X
  vtkMath::Cross(normalizedZ, normalizedX, normalizedY);

  const auto matrix = vtkSmartPointer<vtkMatrix4x4>::New();
  matrix->Identity();

  for (auto i = 0; i < 3; ++i)
    {
    matrix->SetElement(i, 0, normalizedX[i]);
    matrix->SetElement(i, 1, normalizedY[i]);
    matrix->SetElement(i, 2, normalizedZ[i]);
    }

  // Apply the transforms
  const auto transform = vtkSmartPointer<vtkTransform>::New();
  transform->Translate(startPoint);
  transform->Concatenate(matrix);
  transform->Scale(length, length, length);

  // Transform the polydata
  const auto transformPD = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
  transformPD->SetTransform(transform);
  transformPD->SetInputConnection(arrowSource->GetOutputPort());

  const auto writer = vtkSmartPointer<vtkOBJWriter>::New();
  writer->SetFileName(fileName.c_str());
  writer->SetInputConnection(transformPD->GetOutputPort());
  writer->Update();

}
}
