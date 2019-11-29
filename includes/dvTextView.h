#ifndef dv_TextView_h
#define dv_TextView_h

#include <vtkTextProperty.h>
#include <vtkTextActor.h>
#include <vtkSmartPointer.h>

namespace dv {

class TextView {

public:

  std::string m_Text;
  vtkRenderer* m_Renderer;
  vtkSmartPointer<vtkTextActor> m_Actor = nullptr;

  TextView(const std::string& Text, vtkRenderer* Renderer)
  : m_Text(Text)
  , m_Renderer(Renderer)
  , m_Actor(vtkSmartPointer<vtkTextActor>::New())
  {
  this->m_Actor->SetInput(this->m_Text.c_str());
  this->m_Actor->GetTextProperty()->SetFontSize( 60 );
  this->m_Actor->GetTextProperty()->SetColor( 0.0, 0.0, 0.0 );
  this->m_Actor->GetTextProperty()->SetFontFamilyToTimes();
  this->m_Actor->SetPosition( 10, 10 );
  this->m_Renderer->AddActor(this->m_Actor);
  }

  void AddActor() {
    this->m_Renderer->AddActor(this->m_Actor);
  }

  void RemoveActor() {
    this->m_Renderer->RemoveActor(this->m_Actor);
  }

};

}

#endif
