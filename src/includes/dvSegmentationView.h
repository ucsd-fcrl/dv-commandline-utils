#ifndef dv_SegmentationView_h
#define dv_SegmentationView_h

// STD
#include <string>
#include <vector>
#include <array>
#include <map>

// VTK
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkRenderer.h>

namespace dv
{

class SegmentationView
{

  std::vector<unsigned int> m_Labels;
  std::vector<std::array<double, 3>> m_Colors;
  vtkSmartPointer<vtkRenderer> m_Renderer = nullptr;

  double m_SampleRate = 1;
  unsigned int m_Iterations = 10;
  double m_Relaxation = 0.1;
  double m_FeatureAngle = 135.0;

  public:

  SegmentationView(
    const std::string FileName,
    const std::vector<unsigned int> Labels,
    const std::vector<std::array<double, 3>> Colors,
    vtkRenderer* Renderer
    );

  std::map<unsigned int, vtkSmartPointer<vtkActor>> m_Actors;

  void Setup(const std::string file_name);

  void AddAllActors();
  void RemoveAllActors();

};

}

#endif
