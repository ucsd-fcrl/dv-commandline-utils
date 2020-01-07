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

// Custom
#include <dvLabeledITKPointSetWriter.h>
#include <dvITKTriangleMeshToVTKPolyData.h>
#include <dvMeshToQuadEdgeMesh.h>
#include <dvRefineValenceThreeVertices.h>
#include <dvGenerateInitialModel.h>
#include <dvQuickViewPolyData.h>

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

//  if (dv::MeshIncludesValenceThreeVertices<TMesh>( mesh )) {
//    throw 0;
//  }

  // Visualize
  const auto polydata = dv::ITKTriangleMeshToVTKPolyData<TMesh>( mesh );

  dv::QuickViewPolyData( polydata );

  return EXIT_SUCCESS;

}
