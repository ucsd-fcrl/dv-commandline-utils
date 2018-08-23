// ITK
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageToVTKImageFilter.h>

// VTK
#include <vtkDiscreteMarchingCubes.h>
#include <vtkProperty.h>
#include <vtkExtractVOI.h>
#include <vtkMatrix4x4.h>
#include <vtkTransform.h>
#include <vtkPolyDataMapper.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkTransformPolyDataFilter.h>

// Custom
#include <dvGetVTKTransformationMatrixFromITKImage.h>

#include <dvSegmentationView.h>

namespace dv
{

SegmentationView::
SegmentationView(
  const std::string FileName,
  const std::vector<unsigned int> Labels,
  const std::vector<std::array<double, 3>> Colors,
  vtkRenderer* Renderer
  ) : m_Labels(Labels), m_Colors(Colors), m_Renderer(Renderer)
  {
  this->Setup(FileName);
  }

void
SegmentationView::
Setup(const std::string file_name)
{

  using TImage = itk::Image<short,3>;
  using TITKReader = itk::ImageFileReader<TImage>;
  using TITK2VTK = itk::ImageToVTKImageFilter<TImage>;

  const auto reader = TITKReader::New();
  reader->SetFileName( file_name );
  reader->Update();

  const auto mat = vtkSmartPointer<vtkMatrix4x4>::New();
  const auto trans = vtkSmartPointer<vtkTransform>::New();
  dv::GetVTKTransformationMatrixFromITKImage<TImage>( reader->GetOutput(), mat );

  trans->SetMatrix(  mat );
  trans->Translate(
    -trans->GetMatrix()->GetElement(0,3),
    -trans->GetMatrix()->GetElement(1,3),
    -trans->GetMatrix()->GetElement(2,3)
    );

  const auto itk2vtk = TITK2VTK::New();
  itk2vtk->SetInput( reader->GetOutput() );
  itk2vtk->Update();

  const auto voi = vtkSmartPointer<vtkExtractVOI>::New();
  voi->SetInputData( itk2vtk->GetOutput() );
  voi->SetSampleRate( this->m_SampleRate, this->m_SampleRate, this->m_SampleRate );
  voi->SetVOI( itk2vtk->GetOutput()->GetExtent() );

  for (const auto &l : this->m_Labels)
    {
    const auto cube = vtkSmartPointer<vtkDiscreteMarchingCubes>::New();
    cube->SetInputConnection( voi->GetOutputPort() );
    cube->SetValue( 0, l );

    const auto transform = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transform->SetInputConnection( cube->GetOutputPort() );
    transform->SetTransform( trans );

    const auto smooth = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
    smooth->SetInputConnection(transform->GetOutputPort());
    smooth->SetNumberOfIterations(this->m_Iterations);
    smooth->SetRelaxationFactor(this->m_Relaxation);
    smooth->FeatureEdgeSmoothingOn();
    smooth->SetFeatureAngle( this->m_FeatureAngle );
    smooth->BoundarySmoothingOff();

    const auto normals = vtkSmartPointer<vtkPolyDataNormals>::New();
    normals->SetInputConnection(smooth->GetOutputPort());
    normals->ComputePointNormalsOn();
    normals->ComputeCellNormalsOn();
    normals->SplittingOn();
    normals->SetFeatureAngle( this->m_FeatureAngle );
    normals->Update();

    const auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData( normals->GetOutput() );
    mapper->ScalarVisibilityOff();

    const auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper( mapper );
    actor->GetProperty()->SetColor( this->m_Colors.at(l % this->m_Colors.size() ).data() );

    this->m_Actors.emplace(std::make_pair(l, actor));
    }

}

void
SegmentationView::
AddAllActors()
{

  for (const auto &a : this->m_Actors)
    {
    this->m_Renderer->AddActor( a.second );
    }

}

void
SegmentationView::
RemoveAllActors()
{

  for (const auto &a : this->m_Actors)
    {
    this->m_Renderer->RemoveActor( a.second );
    }

}

}

