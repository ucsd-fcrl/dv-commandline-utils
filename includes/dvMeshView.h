#ifndef dv_MeshView_h
#define dv_MeshView_h

#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkOBJReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

namespace dv {

class MeshView {

public:

  std::string m_FileName;
  vtkRenderer* m_Renderer;
  vtkSmartPointer<vtkOBJReader> m_Reader = nullptr;
  vtkSmartPointer<vtkPolyDataMapper> m_Mapper = nullptr;
  vtkSmartPointer<vtkActor> m_Actor = nullptr;

  MeshView(const MeshView& other) {
    this->m_FileName = other.m_FileName;
    this->m_Renderer = other.m_Renderer;
    this->m_Reader   = other.m_Reader;
    this->m_Mapper   = other.m_Mapper;
    this->m_Actor    = other.m_Actor;

    this->Setup();
  }

  MeshView(const std::string& FileName, vtkRenderer* Renderer)
  : m_FileName(FileName)
  , m_Renderer(Renderer)
  , m_Reader(vtkSmartPointer<vtkOBJReader>::New())
  , m_Mapper(vtkSmartPointer<vtkPolyDataMapper>::New())
  , m_Actor(vtkSmartPointer<vtkActor>::New())
  {
  this->Setup();
  }

  void Setup() {
    this->m_Reader->SetFileName(this->m_FileName.c_str());
    this->m_Reader->Update();

    this->m_Mapper->SetInputConnection(this->m_Reader->GetOutputPort());
    this->m_Mapper->ScalarVisibilityOff();

    this->m_Actor->SetMapper(this->m_Mapper);
    this->m_Actor->GetProperty()->SetColor(1.0, 0.0, 0.0);

    this->m_Renderer->AddActor(this->m_Actor);
  }

  void AddActor() {
    this->m_Renderer->AddActor(this->m_Actor);
  }

  void RemoveActor() {
    this->m_Renderer->RemoveActor(this->m_Actor);
  }

  void SetOpacity(const double opacity) {
    this->m_Actor->GetProperty()->SetOpacity( opacity );
  }

  void Update(std::string file_name) {
    this->m_Reader->SetFileName(file_name.c_str());
    this->m_Reader->Update();
  }
};

}

#endif
