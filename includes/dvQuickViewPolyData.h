#ifndef dv_QuickViewPolyData_h
#define dv_QuickViewPolyData_h

// VTK
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

// DV
#include <dvGetLookupTable.h>

namespace dv
{
void
QuickViewPolyData(vtkPolyData* polydata) {


  const auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputData(polydata);

  const auto lut = dv::GetLookupTable();

  mapper->SetScalarRange(0, 8);
  mapper->SetLookupTable(lut);
  const auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  const auto renderer = vtkSmartPointer<vtkRenderer>::New();
  const auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  const auto renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);
  renderer->AddActor(actor);
  renderer->SetBackground(1, 1, 1);  
  renderWindow->Render();
  renderWindowInteractor->Start();

}
}

#endif
