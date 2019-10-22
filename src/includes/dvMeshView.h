#ifndef dv_MeshView_h
#define dv_MeshView_h

#include <vtkOBJReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>

namespace dv
{

class MeshView
{

  vtkSmartPointer<vtkOBJReader> m_Reader = nullptr;
  vtkSmartPointer<vtkPolyDataMapper> m_Mapper = nullptr;
  vtkSmartPointer<vtkActor> m_Actor = nullptr;

  public:

  MeshView()
    {
    this->m_Reader = vtkSmartPointer<vtkOBJReader>::New();
    this->m_Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    this->m_Actor = vtkSmartPointer<vtkActor>::New();
    }

  void Setup(
    const std::string file_name,
    vtkRenderer* renderer
    )
    {
    this->m_Reader->SetFileName( file_name.c_str() );
    this->m_Reader->Update();

    this->m_Mapper->SetInputConnection( this->m_Reader->GetOutputPort() );
    this->m_Mapper->ScalarVisibilityOff();

    this->m_Actor->SetMapper( this->m_Mapper );
    this->m_Actor->GetProperty()->SetColor( 1.0, 0.0, 0.0 );

    renderer->AddActor( this->m_Actor );
    }

  void Update(std::string file_name)
    {
    this->m_Reader->SetFileName( file_name.c_str() );
    this->m_Reader->Update();
    }

};

}

#endif
