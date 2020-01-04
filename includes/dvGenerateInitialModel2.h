#ifndef dv_GenerateInitialModel2_h
#define dv_GenerateInitialModel2_h

// ITK
#include <itkImageFileReader.h>
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
#include <dvITKTriangleMeshToVTKPolyData.h>

namespace dv {

void
GenerateInitialModel2(const std::string& inputSegmentationName,
                     const std::string& outputMeshName,
                     const unsigned int& count,
                     const double& sigma)
{

  using TPixel = unsigned char;
  const unsigned int Dimension = 3;
  using TCoordinate = float;
  using TMesh = itk::QuadEdgeMesh<TCoordinate, Dimension>;

  using TImage = itk::Image<TPixel, Dimension>;
  using TReader = itk::ImageFileReader<TImage>;
  using TPad = itk::ConstantPadImageFilter<TImage, TImage>;
  using TExtract = itk::CuberilleImageToMeshFilter< TImage, TMesh >;

  using TNoise = itk::AdditiveGaussianNoiseQuadEdgeMeshFilter<TMesh>;
  using TCriterion = itk::NumberOfFacesCriterion<TMesh>;
  using TDecimation = itk::
    SquaredEdgeLengthDecimationQuadEdgeMeshFilter<TMesh, TMesh, TCriterion>;
  using TDelaunay = itk::DelaunayConformingQuadEdgeMeshFilter<TMesh>;
  using TLoop =
    itk::LoopTriangleCellSubdivisionQuadEdgeMeshFilter<TMesh, TMesh>;

  const auto reader = TReader::New();
  reader->SetFileName( inputSegmentationName );
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
  noise->SetSigma(sigma);

  const auto criterion = TCriterion::New();
  const auto decimate = TDecimation::New();

  criterion->SetNumberOfElements(count);

  decimate->SetInput(noise->GetOutput());
  decimate->SetCriterion(criterion);

  const auto delaunay = TDelaunay::New();
  delaunay->SetInput(decimate->GetOutput());

  const auto loop = TLoop::New();
  loop->SetInput( delaunay->GetOutput() );
  loop->Update();

  const auto poly_data = dv::ITKTriangleMeshToVTKPolyData< TMesh >( loop->GetOutput() );

  const auto writer = vtkSmartPointer< vtkPolyDataWriter >::New();
  writer->SetInputData( poly_data );
  writer->SetFileName(outputMeshName.c_str());
  writer->Update();
}

}

#endif
