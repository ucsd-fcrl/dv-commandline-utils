#ifndef dv_GenerateInitialModel_h
#define dv_GenerateInitialModel_h

// ITK
#include <itkImageFileReader.h>
#include <itkBinaryBallStructuringElement.h>
#include <itkBinaryMorphologicalClosingImageFilter.h>
#include <itkConstantPadImageFilter.h>
#include <itkCuberilleImageToMeshFilter.h>
#include <itkQuadEdgeMesh.h>
#include <itkAdditiveGaussianNoiseQuadEdgeMeshFilter.h>
#include <itkQuadEdgeMeshDecimationCriteria.h>
#include <itkSquaredEdgeLengthDecimationQuadEdgeMeshFilter.h>
#include <itkDelaunayConformingQuadEdgeMeshFilter.h>
#include <itkLoopTriangleCellSubdivisionQuadEdgeMeshFilter.h>

// VTK
#include <vtkPolyDataWriter.h>

// Custom
#include <itkEnforceBoundaryBetweenLabelsImageFilter.h>
#include <itkFillHolesInSegmentationImageFilter.h>
#include <itkExtractConnectedComponentsImageFilter.h>
#include <dvITKTriangleMeshToVTKPolyData.h>
#include <dvRefineValenceThreeVertices.h>

namespace dv {

void
GenerateInitialModel(const std::string& inputSegmentationName,
                     const std::string& outputMeshName,
                     const unsigned int& count,
                     const double& sigma,
                     const unsigned int& radius)
{

  using TPixel = unsigned char;
  const unsigned int Dimension = 3;
  using TCoordinate = float;
  using TMesh = itk::QuadEdgeMesh<TCoordinate, Dimension>;

  using TImage = itk::Image<TPixel, Dimension>;
  using TReader = itk::ImageFileReader<TImage>;
  using TKernel = itk::BinaryBallStructuringElement<TPixel, Dimension>;
  using CloseType =
    itk::BinaryMorphologicalClosingImageFilter<TImage, TImage, TKernel>;
  using TFill = itk::FillHolesInSegmentationImageFilter<TImage>;
  using TEnforce = itk::EnforceBoundaryBetweenLabelsImageFilter<TImage>;
  using TConnected = itk::ExtractConnectedComponentsImageFilter<TImage>;
  using TPad = itk::ConstantPadImageFilter<TImage, TImage>;
  using TCuberille = itk::CuberilleImageToMeshFilter< TImage, TMesh >;

  using TNoise = itk::AdditiveGaussianNoiseQuadEdgeMeshFilter<TMesh>;
  using TCriterion = itk::NumberOfFacesCriterion<TMesh>;
  using TDecimation = itk::
    SquaredEdgeLengthDecimationQuadEdgeMeshFilter<TMesh, TMesh, TCriterion>;
//  using TDelaunay = itk::DelaunayConformingQuadEdgeMeshFilter<TMesh>;
  using TLoop =
    itk::LoopTriangleCellSubdivisionQuadEdgeMeshFilter<TMesh, TMesh>;

  const auto reader = TReader::New();
  reader->SetFileName( inputSegmentationName );

  const auto enforce0 = TEnforce::New();
  enforce0->SetInput( reader->GetOutput() );
  enforce0->SetLabels1({4, 5});
  enforce0->SetLabels2({2, 3, 6, 7, 8, 9});

  const auto enforce1 = TEnforce::New();
  enforce1->SetInput( enforce0->GetOutput() );
  enforce1->SetLabels1({3});
  enforce1->SetLabels2({1, 6, 7, 8, 9});

  TKernel closeKernel;
  closeKernel.SetRadius(radius);
  closeKernel.CreateStructuringElement();

  const auto closing = CloseType::New();
  closing->SetInput( enforce1->GetOutput() );
  closing->SetKernel( closeKernel );
  closing->SetForegroundValue( 1 );

  const auto connected = TConnected::New();
  connected->SetInput( closing->GetOutput() );

  const auto fill = TFill::New();
  fill->SetInput( connected->GetOutput() );

  typename TImage::SizeType padding;
  padding.Fill(1);

  const auto pad = TPad::New();
  pad->SetInput(fill->GetOutput());
  pad->SetPadUpperBound(padding);
  pad->SetPadLowerBound(padding);
  pad->SetConstant(static_cast<TPixel>(0));

  const auto cuberille = TCuberille::New();
  cuberille->SetInput( pad->GetOutput() );
  cuberille->GenerateTriangleFacesOn();
  cuberille->ProjectVerticesToIsoSurfaceOff();
  cuberille->SavePixelAsCellDataOn();

  const auto noise = TNoise::New();
  noise->SetInput( cuberille->GetOutput() );
  noise->SetSigma(sigma);

  const auto criterion = TCriterion::New();
  const auto decimate = TDecimation::New();

  criterion->SetNumberOfElements(count);

  decimate->SetInput(noise->GetOutput());
  decimate->SetCriterion(criterion);

//  const auto mesh = TMesh::New();
//  mesh->Graft( decimate->GetOutput() );
//  mesh->DisconnectPipeline();
//
//  // FIXME: Delaunay triangulation segfaults with certain parameters
//  const auto delaunay = TDelaunay::New();
//  delaunay->SetInput( decimate->GetOutput() );

//  // TODO: Refactor into proper itk::MeshToMeshFilter
//  dv::RefineValenceThreeVertices< TMesh >( mesh );

  const auto loop = TLoop::New();
  loop->SetInput( decimate->GetOutput() );
  loop->Update();

  const auto poly_data = dv::ITKTriangleMeshToVTKPolyData< TMesh >( loop->GetOutput() );

  const auto writer = vtkSmartPointer< vtkPolyDataWriter >::New();
  writer->SetInputData( poly_data );
  writer->SetFileName(outputMeshName.c_str());
  writer->Update();

}

}

#endif
