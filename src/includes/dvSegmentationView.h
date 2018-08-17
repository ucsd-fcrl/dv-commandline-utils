#ifndef dv_SegmentationView_h
#define dv_SegmentationView_h

// ITK
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageToVTKImageFilter.h>

// VTK
#include <vtkDiscreteMarchingCubes.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkExtractVOI.h>
#include <vtkMatrix4x4.h>
#include <vtkTransform.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>

// Custom
#include <dvGetVTKTransformationMatrixFromITKImage.h>

namespace dv
{

class SegmentationView
{

  using TImage = itk::Image<short,3>;
  using TITKReader = itk::ImageFileReader<TImage>;
  using TITK2VTK = itk::ImageToVTKImageFilter<TImage>;

  TITKReader::Pointer m_SegmentationReader = nullptr;
  TITK2VTK::Pointer m_ITK2VTK = nullptr;
  std::vector<vtkSmartPointer<vtkDiscreteMarchingCubes>> m_Cubes;
  vtkSmartPointer<vtkMatrix4x4> fMat = nullptr;
  vtkSmartPointer<vtkMatrix4x4> bMat = nullptr;
  vtkSmartPointer<vtkTransform> fTrans = nullptr;
  vtkSmartPointer<vtkTransform> bTrans = nullptr;
  vtkSmartPointer<vtkExtractVOI> m_VOI = nullptr;
  double SampleRate = 1;

  public:

  SegmentationView()
    {
    this->m_SegmentationReader = TITKReader::New();

    this->fMat = vtkSmartPointer<vtkMatrix4x4>::New();
    this->bMat = vtkSmartPointer<vtkMatrix4x4>::New();

    this->fTrans = vtkSmartPointer<vtkTransform>::New();
    this->bTrans = vtkSmartPointer<vtkTransform>::New();

    this->m_ITK2VTK = TITK2VTK::New();

    this->m_VOI = vtkSmartPointer<vtkExtractVOI>::New();
    }

  void Setup(
    const double SampleRate,
    const std::vector<unsigned int> labels,
    std::vector<std::array<double, 3>> colors,
    const std::string file_name,
    vtkRenderer* renderer
    )
    {

    this->m_SegmentationReader->SetFileName( file_name );
    this->m_SegmentationReader->Update();

    dv::GetVTKTransformationMatrixFromITKImage<TImage>( this->m_SegmentationReader->GetOutput(), fMat);

    this->fTrans->SetMatrix(  fMat );
    this->fTrans->GetInverse( bMat );
    this->bTrans->SetMatrix(  bMat );

    this->m_ITK2VTK->SetInput( this->m_SegmentationReader->GetOutput() );
    this->m_ITK2VTK->Update();

    this->m_VOI->SetInputData( this->m_ITK2VTK->GetOutput() );
    this->m_VOI->SetSampleRate( SampleRate, SampleRate, SampleRate );
    this->m_VOI->SetVOI( this->m_ITK2VTK->GetOutput()->GetExtent() );

    for (const auto &l : labels)
      {
      const auto cube = vtkSmartPointer<vtkDiscreteMarchingCubes>::New();
      cube->SetInputConnection( m_VOI->GetOutputPort() );

      cube->SetValue( 0, l );

      const auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
      mapper->SetInputConnection( cube->GetOutputPort() );
      mapper->ScalarVisibilityOff();
 
      const auto actor = vtkSmartPointer<vtkActor>::New();
      actor->SetUserMatrix( fTrans->GetMatrix() );
      actor->SetPosition(-fTrans->GetMatrix()->GetElement(0,3),
                         -fTrans->GetMatrix()->GetElement(1,3),
                         -fTrans->GetMatrix()->GetElement(2,3));

      actor->SetMapper( mapper );
      actor->GetProperty()->SetColor( colors.at(l % colors.size() ).data() );
      renderer->AddActor( actor );
      this->m_Cubes.push_back( cube );
      }
    }

  void Update(std::string file_name)
    {
    this->m_SegmentationReader->SetFileName( file_name.c_str() );
    this->m_SegmentationReader->Update();
    this->m_ITK2VTK->Update();
    for (const auto &c : this->m_Cubes) c->Update();
    }

};

}

#endif
