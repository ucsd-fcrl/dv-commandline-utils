#ifndef dv_ExtractInterfaceBetweenSegments_h
#define dv_ExtractInterfaceBetweenSegments_h

// ITK
#include <itkMesh.h>
#include <itkImageFileReader.h>
#include <itkMeshFileWriter.h>
#include <itkSTLMeshIO.h>
#include <itkCuberilleImageToMeshFilter.h>
#include <itkBinaryMask3DMeshSource.h>
#include <itkStatisticsImageFilter.h>
#include <itkMacro.h>
#include <itkBinaryDilateImageFilter.h>
#include <itkBinaryBallStructuringElement.h>
#include <itkAndImageFilter.h>

// Custom
#include "itkExtractLabelsImageFilter.h"

namespace dv
{

template<unsigned int Dimension, typename TPixel, typename TCoordinate>
void
ExtractInterfaceBetweenSegments(const std::string &IImage, const std::string &OMesh, const std::set<TPixel> &labels_1, const std::set<TPixel> &labels_2)
{

  using TImage = itk::Image< TPixel, Dimension >;
  using TMesh   = itk::Mesh< TCoordinate, Dimension >;

  using ReaderType = itk::ImageFileReader< TImage >;
  using FilterType = itk::ExtractLabelsImageFilter<TImage>;
  using TMeshSource = itk::BinaryMask3DMeshSource< TImage, TMesh >;
  using TCuberille = itk::CuberilleImageToMeshFilter< TImage, TMesh >;
  using TStats = itk::StatisticsImageFilter< TImage >;
  using TBall = itk::BinaryBallStructuringElement< TPixel, Dimension>;
  using TDilate = itk::BinaryDilateImageFilter< TImage, TImage, TBall >;
  using TAnd = itk::AndImageFilter< TImage >;

  using TWriter = itk::MeshFileWriter< TMesh >;

  //
  // Reader
  //

  const auto reader = ReaderType::New();
  reader->SetFileName( IImage );

  //
  // make unary filter which checks if pixels are in set
  //
  const auto filter_1 = FilterType::New();
  filter_1->SetLabels( labels_1 );
  filter_1->SetInput( reader->GetOutput() );
  filter_1->Update();

  const auto binary_1 = TImage::New();
  binary_1->Graft( filter_1->GetOutput() );
  binary_1->DisconnectPipeline();

  const auto filter_2 = FilterType::New();
  filter_2->SetLabels( labels_2 );
  filter_2->SetInput( reader->GetOutput() );
  filter_2->Update();

  const auto binary_2 = TImage::New();
  binary_2->Graft( filter_2->GetOutput() );
  binary_2->DisconnectPipeline();

  // Check

    {
    const auto stats = TStats::New();
    stats->SetInput( binary_1 );
    stats->Update();
    itkAssertOrThrowMacro(0 < stats->GetSum(), "Segmentation image 1 is empty.");
    }

    {
    const auto stats = TStats::New();
    stats->SetInput( binary_2 );
    stats->Update();
    itkAssertOrThrowMacro(0 < stats->GetSum(), "Segmentation image 2 is empty.");
    }

  // Dilate

  TBall ball;
  ball.SetRadius( 1 );
  ball.CreateStructuringElement();

  const auto dilate_1 = TDilate::New();
  dilate_1->SetInput( binary_1 );
  dilate_1->SetKernel( ball );
  dilate_1->SetDilateValue( 1 );

  const auto dilate_2 = TDilate::New();
  dilate_2->SetInput( binary_2 );
  dilate_2->SetKernel( ball );
  dilate_2->SetDilateValue( 1 );

  // Intersection
  const auto intersection = TAnd::New();
  intersection->SetInput1( dilate_1->GetOutput() );
  intersection->SetInput2( dilate_2->GetOutput() );
  intersection->Update();

    {
    const auto stats = TStats::New();
    stats->SetInput( intersection->GetOutput() );
    stats->Update();
    itkAssertOrThrowMacro(0 < stats->GetSum(), "Intersection image is empty.");
    }
  
  //
  // Meshing
  //
//  if (USE_CUBERILLE)
//    {
//  const auto meshSource = TMeshSource::New();
//  const TPixel objectValue = static_cast<TPixel>( 1 );
//  meshSource->SetObjectValue( objectValue );
//  meshSource->SetInput( intersection->GetOutput() );
//    }
//  else
//    {
    const auto meshSource = TCuberille::New();
    meshSource->SetInput( intersection->GetOutput() );
    meshSource->SetIsoSurfaceValue( 0.5 );
    meshSource->SetProjectVertexSurfaceDistanceThreshold( 0.1 );
//    }

  //
  // Writer
  //

  const auto writer = TWriter::New();
  writer->SetInput( meshSource->GetOutput() );
  writer->SetFileName( OMesh );

  const auto o_ext = OMesh.substr( OMesh.size() - 3, 3);
  if (o_ext == "stl" || o_ext == "STL")
  {
    writer->SetMeshIO( itk::STLMeshIO::New() );
  }

  writer->Update();

}

}

#endif
