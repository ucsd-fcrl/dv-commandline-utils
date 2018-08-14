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
  writer->SetFileName("arrow.obj");
  writer->SetInputConnection(transformPD->GetOutputPort());
  writer->Update();

}
}

//int main(int, char *[])
//{
//  std::array<double, 3> startPoint{{10.0, 10.0, 10.0}};
//  std::array<double, 3> endPoint{{5.0, 5.0, 5.0}};
//  const std::string fileName = "arrow.obj";
//  const int tipResolution = 50;
//  const int shaftResolution = 50;
//
//  dv::ArrowWithStartAndEnd(startPoint, endPoint, fileName, tipResolution, shaftResolution);
//
//  const auto reader = vtkSmartPointer<vtkOBJReader>::New();
//  reader->SetFileName( fileName.c_str() );
//
//  const auto colors = vtkSmartPointer<vtkNamedColors>::New();
//
//  //Create a mapper and actor for the arrow
//  vtkSmartPointer<vtkPolyDataMapper> mapper =
//    vtkSmartPointer<vtkPolyDataMapper>::New();
//  vtkSmartPointer<vtkActor> actor =
//    vtkSmartPointer<vtkActor>::New();
//#ifdef USER_MATRIX
//  mapper->SetInputConnection(reader->GetOutputPort());
////  actor->SetUserMatrix(transform->GetMatrix());
//#else
////  mapper->SetInputConnection(transformPD->GetOutputPort());
//#endif
//  actor->SetMapper(mapper);
//  actor->GetProperty()->SetColor(colors->GetColor3d("Cyan").GetData());
//
//  // Create spheres for start and end point
//  vtkSmartPointer<vtkSphereSource> sphereStartSource =
//    vtkSmartPointer<vtkSphereSource>::New();
//    sphereStartSource->SetCenter(startPoint);
//    sphereStartSource->SetRadius(0.8);
//  vtkSmartPointer<vtkPolyDataMapper> sphereStartMapper =
//    vtkSmartPointer<vtkPolyDataMapper>::New();
//  sphereStartMapper->SetInputConnection(sphereStartSource->GetOutputPort());
//  vtkSmartPointer<vtkActor> sphereStart =
//    vtkSmartPointer<vtkActor>::New();
//  sphereStart->SetMapper(sphereStartMapper);
//  sphereStart->GetProperty()->SetColor(colors->GetColor3d("Yellow").GetData());
//
//  vtkSmartPointer<vtkSphereSource> sphereEndSource =
//    vtkSmartPointer<vtkSphereSource>::New();
//    sphereEndSource->SetCenter(endPoint);
//    sphereEndSource->SetRadius(0.8);
//  vtkSmartPointer<vtkPolyDataMapper> sphereEndMapper =
//    vtkSmartPointer<vtkPolyDataMapper>::New();
//  sphereEndMapper->SetInputConnection(sphereEndSource->GetOutputPort());
//  vtkSmartPointer<vtkActor> sphereEnd =
//    vtkSmartPointer<vtkActor>::New();
//  sphereEnd->SetMapper(sphereEndMapper);
//  sphereEnd->GetProperty()->SetColor(colors->GetColor3d("Magenta").GetData());
//
//  //Create a renderer, render window, and interactor
//  vtkSmartPointer<vtkRenderer> renderer =
//    vtkSmartPointer<vtkRenderer>::New();
//  vtkSmartPointer<vtkRenderWindow> renderWindow =
//    vtkSmartPointer<vtkRenderWindow>::New();
//  renderWindow->AddRenderer(renderer);
//  renderWindow->SetWindowName("Oriented Arrow");
//  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
//    vtkSmartPointer<vtkRenderWindowInteractor>::New();
//  renderWindowInteractor->SetRenderWindow(renderWindow);
//
//  const auto axes = vtkSmartPointer<vtkAxesActor>::New();
//
//  //Add the actor to the scene
//  renderer->AddActor(axes);
//  renderer->AddActor(actor);
//  renderer->AddActor(sphereStart);
//  renderer->AddActor(sphereEnd);
//  renderer->SetBackground(colors->GetColor3d("BkgColor").GetData());
//
//  //Render and interact
//  renderWindow->Render();
//  renderWindowInteractor->Start();
//
//  return EXIT_SUCCESS;
//}
