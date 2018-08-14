#ifndef dv_EnforceBoundaryBetweenLabels_h
#define dv_EnforceBoundaryBetweenLabels_h

// ITK
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkStatisticsImageFilter.h>
#include <itkMacro.h>
#include <itkBinaryDilateImageFilter.h>
#include <itkBinaryBallStructuringElement.h>
#include <itkAndImageFilter.h>
#include <itkMaskImageFilter.h>

// Custom
#include "itkExtractLabelsImageFilter.h"

namespace dv
{

template<unsigned int Dimension, typename TPixel, typename TCoordinate>
void
EnforceBoundaryBetweenLabels(
  const std::string &IImage,
  const std::string &OMesh,
  const std::set<TPixel> &labels_1,
  const std::set<TPixel> &labels_2,
  const short radius_1,
  const short radius_2,
  const short replacement)
{

  using TImage = itk::Image< TPixel, Dimension >;
  using TReader = itk::ImageFileReader< TImage >;
  using TWriter = itk::ImageFileWriter< TImage >;

  using TFilter = itk::ExtractLabelsImageFilter<TImage>;
  using TStats = itk::StatisticsImageFilter< TImage >;
  using TBall = itk::BinaryBallStructuringElement< TPixel, Dimension>;
  using TDilate = itk::BinaryDilateImageFilter< TImage, TImage, TBall >;
  using TAnd = itk::AndImageFilter< TImage >;
  using TMask = itk::MaskImageFilter< TImage, TImage >;

  //
  // Reader
  //

  const auto reader = TReader::New();
  reader->SetFileName( IImage );

  //
  // make unary filter which checks if pixels are in set
  //
  const auto filter_1 = TFilter::New();
  filter_1->SetLabels( labels_1 );
  filter_1->SetInput( reader->GetOutput() );
  filter_1->Update();

  const auto binary_1 = TImage::New();
  binary_1->Graft( filter_1->GetOutput() );
  binary_1->DisconnectPipeline();

  const auto filter_2 = TFilter::New();
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

    if (stats->GetSum() == 0)
      {
      std::cerr << "WARNING: No pixels corresponding to label set 1; output is unchanged." << std::endl;

      const auto writer = TWriter::New();
      writer->SetInput( reader->GetOutput() );
      writer->SetFileName( OMesh );
      writer->Update();

      return;
      }
    }

    {
    const auto stats = TStats::New();
    stats->SetInput( binary_2 );
    stats->Update();

    if (stats->GetSum() == 0)
      {
      std::cerr << "WARNING: No pixels corresponding to label set 1; output is unchanged." << std::endl;

      const auto writer = TWriter::New();
      writer->SetInput( reader->GetOutput() );
      writer->SetFileName( OMesh );
      writer->Update();

      return;
      }
    }

  // Dilate

  TBall ball_1;
  ball_1.SetRadius( radius_1 );
  ball_1.CreateStructuringElement();

  const auto dilate_1 = TDilate::New();
  dilate_1->SetInput( binary_1 );
  dilate_1->SetKernel( ball_1 );
  dilate_1->SetDilateValue( 1 );

  TBall ball_2;
  ball_2.SetRadius( radius_2 );
  ball_2.CreateStructuringElement();

  const auto dilate_2 = TDilate::New();
  dilate_2->SetInput( binary_2 );
  dilate_2->SetKernel( ball_2 );
  dilate_2->SetDilateValue( 1 );

  // Intersection
  const auto intersection = TAnd::New();
  intersection->SetInput1( dilate_1->GetOutput() );
  intersection->SetInput2( dilate_2->GetOutput() );

  const auto mask = TMask::New();
  mask->SetInput( reader->GetOutput() );
  mask->SetMaskImage( intersection->GetOutput() );
  mask->SetMaskingValue( 1 );
  mask->SetOutsideValue( replacement );

  const auto writer = TWriter::New();
  writer->SetInput( mask->GetOutput() );
  writer->SetFileName( OMesh );
  writer->Update();

}

}

#endif
