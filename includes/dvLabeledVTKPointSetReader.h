#ifndef dv_LabeledVTKPointSetReader_h
#define dv_LabeledVTKPointSetReader_h

// STD
#include <iostream>
#include <fstream>
#include <map>

// VTK
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkUnsignedCharArray.h>
#include <vtkPointData.h>

// Custom
#include <dvGetListOfColors.h>

namespace dv {
vtkSmartPointer<vtkPolyData>
LabeledVTKPointSetReader(const std::string&file_name) {

  const auto points = vtkSmartPointer<vtkPoints>::New();
  const auto colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
  colors->SetNumberOfComponents(3);
  colors->SetName ("Colors");
  const auto label_colors = dv::GetListOfColors(false);

  std::ifstream file(file_name);

  double x, y, z;
  size_t l;

  while (file >> x >> y >> z >> l) {
    points->InsertNextPoint(x, y, z);
    colors->InsertNextTuple(label_colors.at(l).data());
  }

  file.close();

  const auto pointsPolyData = vtkSmartPointer<vtkPolyData>::New();
  pointsPolyData->SetPoints( points );

  const auto vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
  vertexFilter->SetInputData(pointsPolyData);
  vertexFilter->Update();

  const auto polydata = vtkSmartPointer<vtkPolyData>::New();
  polydata->ShallowCopy(vertexFilter->GetOutput());
  polydata->GetPointData()->SetScalars(colors);

  return polydata;

}
}

#endif
