#ifndef dv_SegmentationView_h
#define dv_SegmentationView_h

// STD
#include <array>
#include <map>
#include <string>
#include <vector>

// VTK
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkProperty.h>

namespace dv {

class SegmentationView {

  static constexpr double m_SampleRate = 1;
  static constexpr unsigned int m_Iterations = 10;
  static constexpr double m_Relaxation = 0.1;
  static constexpr double m_FeatureAngle = 135.0;

public:

  std::string m_FileName;
  vtkSmartPointer<vtkRenderer> m_Renderer;
  std::vector<unsigned int> m_Labels;
  std::vector<std::array<double, 3>> m_Colors;

  SegmentationView(const SegmentationView& other);

  void Setup();

  SegmentationView(const std::string FileName,
                   vtkRenderer* Renderer,
                   const std::vector<unsigned int> Labels,
                   const std::vector<std::array<double, 3>> Colors);

  std::map<unsigned int, vtkSmartPointer<vtkActor>> m_Actors;

  void AddAllActors();
  void RemoveAllActors();

  void SetOpacity(const double opacity) {
    for (auto &a : this->m_Actors) {
      a.second->GetProperty()->SetOpacity( opacity );
    }
  }

};

}

#endif
