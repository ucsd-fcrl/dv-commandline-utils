// STD
#include <fstream>

// ITK
#include <itkVTKPolyDataWriter.h>
#include <itkPointSet.h>
#include <itkQuadEdgeMesh.h>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkConstantPadImageFilter.h>
#include <itkCuberilleImageToMeshFilter.h>
#include <itkAdditiveGaussianNoiseQuadEdgeMeshFilter.h>
#include <itkQuadEdgeMeshDecimationCriteria.h>
#include <itkSquaredEdgeLengthDecimationQuadEdgeMeshFilter.h>
#include <itkDelaunayConformingQuadEdgeMeshFilter.h>
#include <itkLoopTriangleCellSubdivisionQuadEdgeMeshFilter.h>
#include <itkVTKPolyDataWriter.h>

#include <dvGenerateInitialModel2.h>

// VTK
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkLookupTable.h>
#include <vtkLookupTable.h>
#include <vtkNamedColors.h>

// Custom
#include <dvLabeledITKPointSetWriter.h>
#include <dvITKTriangleMeshToVTKPolyData.h>
#include <dvMeshToQuadEdgeMesh.h>
#include <dvRefineValenceThreeVertices.h>

using TPixel = unsigned char;
using TCoordinate = float;
const unsigned int Dimension = 3;
using TImage = itk::Image<TPixel, Dimension>;
using TMesh = itk::QuadEdgeMesh<TCoordinate, Dimension>;
using TPointSet = itk::PointSet<TCoordinate, Dimension>;
using TReader = itk::ImageFileReader<TImage>;
using TPad = itk::ConstantPadImageFilter<TImage, TImage>;
using TExtract = itk::CuberilleImageToMeshFilter< TImage, TMesh >;
using TNoise = itk::AdditiveGaussianNoiseQuadEdgeMeshFilter< TMesh >;
using TCriterion = itk::NumberOfFacesCriterion<TMesh>;
using TDecimation = itk::SquaredEdgeLengthDecimationQuadEdgeMeshFilter<TMesh, TMesh, TCriterion>;
using TDelaunay = itk::DelaunayConformingQuadEdgeMeshFilter<TMesh>;
using TLoop = itk::LoopTriangleCellSubdivisionQuadEdgeMeshFilter<TMesh, TMesh>;

int main() {

  const std::string file_name = "/Users/davisvigneault/Desktop/SiSSR-data/input2/seg-nii/0.nii.gz";
  const double sigma = 0.25;
  const unsigned int count = 50;

  const auto reader = TReader::New();
  reader->SetFileName( file_name );
  reader->Update();

  const auto image = TImage::New();
  image->Graft( reader->GetOutput() );
  typename TImage::SizeType padding;
  padding.Fill(1);

  const auto pad = TPad::New();
  pad->SetInput(image);
  pad->SetPadUpperBound(padding);
  pad->SetPadLowerBound(padding);
  pad->SetConstant(static_cast<TPixel>(0));

  const auto extract = TExtract::New();
  extract->SetInput( pad->GetOutput() );
  extract->GenerateTriangleFacesOn();
  extract->ProjectVerticesToIsoSurfaceOff();
  extract->SavePixelAsCellDataOn();

  const auto noise = TNoise::New();
  noise->SetInput( extract->GetOutput() );
  noise->SetSigma( sigma );

  const auto criterion = TCriterion::New();
  const auto decimate = TDecimation::New();
  criterion->SetNumberOfElements( count );

  decimate->SetInput( noise->GetOutput() );
  decimate->SetCriterion(criterion);

  const auto delaunay = TDelaunay::New();
  delaunay->SetInput(decimate->GetOutput());

  const auto loop = TLoop::New();
  loop->SetInput( delaunay->GetOutput() );
  loop->Update();

  const auto mesh = TMesh::New();
  mesh->Graft( loop->GetOutput() );

  if (dv::MeshIncludesValenceThreeVertices<TMesh>( mesh )) {
    throw 0;
  }

  // Visualize
  const auto polydata = dv::ITKTriangleMeshToVTKPolyData<TMesh>( mesh );

  // Create a lookup table to map cell data to colors
  const auto lut = vtkSmartPointer<vtkLookupTable>::New();
  lut->SetNumberOfTableValues(9);
  lut->Build();

  // Fill in a few known colors, the rest will be generated if needed
  vtkSmartPointer<vtkNamedColors> colors =
    vtkSmartPointer<vtkNamedColors>::New();
  lut->SetTableValue(0, colors->GetColor4d("Black").GetData());
  lut->SetTableValue(1, colors->GetColor4d("Red").GetData());
  lut->SetTableValue(2, colors->GetColor4d("Orange").GetData());
  lut->SetTableValue(3, colors->GetColor4d("Yellow").GetData());
  lut->SetTableValue(4, colors->GetColor4d("Green").GetData());
  lut->SetTableValue(5, colors->GetColor4d("Blue").GetData());
  lut->SetTableValue(6, colors->GetColor4d("Purple").GetData());
  lut->SetTableValue(7, colors->GetColor4d("Cyan").GetData());
  lut->SetTableValue(8, colors->GetColor4d("Magenta").GetData());

  const auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputData(polydata);
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

  return EXIT_SUCCESS;

}
