#ifndef dv_QuickViewPolyData_h
#define dv_QuickViewPolyData_h

// VTK
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkNamedColors.h>

// DV
#include <dvGetLookupTable.h>

namespace dv
{
void
QuickViewSideBySidePolyData(vtkPolyData* i_polydata, vtkPolyData* o_polydata) {

  const auto i_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  i_mapper->SetInputData( i_polydata );
  const auto i_actor = vtkSmartPointer<vtkActor>::New();
  i_actor->SetMapper(i_mapper);

  const auto o_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  o_mapper->SetInputData( o_polydata );
  const auto o_actor = vtkSmartPointer<vtkActor>::New();
  o_actor->SetMapper(o_mapper);

  const auto lut = dv::GetLookupTable();

  i_mapper->SetScalarRange(0, 8);
  i_mapper->SetLookupTable(lut);
  o_mapper->SetScalarRange(0, 8);
  o_mapper->SetLookupTable(lut);

  const auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->SetSize(600, 300);
  
  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(renderWindow);
  
  double i_viewport[4] = {0.0, 0.0, 0.5, 1.0};
  double o_viewport[4] = {0.5, 0.0, 1.0, 1.0};
  
  const auto i_renderer = vtkSmartPointer<vtkRenderer>::New();
  renderWindow->AddRenderer(i_renderer);
  i_renderer->SetViewport(i_viewport);
  i_renderer->SetBackground(1, 1, 1);  

  const auto o_renderer = vtkSmartPointer<vtkRenderer>::New();
  renderWindow->AddRenderer(o_renderer);
  o_renderer->SetViewport(o_viewport);
  o_renderer->SetBackground(1, 1, 1);  
      
  i_renderer->AddActor(i_actor);
  o_renderer->AddActor(o_actor);

  o_renderer->SetActiveCamera( i_renderer->GetActiveCamera() );

  i_renderer->ResetCamera();
  o_renderer->ResetCamera();

  renderWindow->Render();
  interactor->Start();

}
}

#endif
