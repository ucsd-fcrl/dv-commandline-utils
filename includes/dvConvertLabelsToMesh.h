#ifndef dv_ConvertLabelsToMesh_h
#define dv_ConvertLabelsToMesh_h

// ITK
#include <itkMesh.h>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkMeshFileWriter.h>
#include <itkBinaryMask3DMeshSource.h>
#include <itkConnectedComponentImageFilter.h>
#include <itkLabelShapeKeepNObjectsImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkConstantPadImageFilter.h>
#include <itkBinaryFillholeImageFilter.h>

// Custom
#include "itkExtractLabelsImageFilter.h"

namespace dv
{

template<unsigned int Dimension, typename TPixel, typename TCoordinate>
void
ConvertLabelsToMesh(const std::string &IImage, const std::set<TPixel> &LabelSet, const std::string &OMesh)
{

  using TImage = itk::Image< TPixel, Dimension >;
  using TMesh   = itk::Mesh< TCoordinate, Dimension >;

  using ReaderType = itk::ImageFileReader< TImage >;
  using TPad = itk::ConstantPadImageFilter< TImage, TImage >;
  using TExtract = itk::ExtractLabelsImageFilter<TImage>;
  using TLabel  = itk::ConnectedComponentImageFilter< TImage, TImage >;
  using TKeep   = itk::LabelShapeKeepNObjectsImageFilter< TImage >;
  using TThresh = itk::BinaryThresholdImageFilter< TImage, TImage >;
  using TMeshSource = itk::BinaryMask3DMeshSource< TImage, TMesh >;
  using TFill = itk::BinaryFillholeImageFilter< TImage >;

  using TWriter = itk::MeshFileWriter< TMesh >;

  //
  // Reader
  //

  const auto reader = ReaderType::New();
  reader->SetFileName( IImage );

  //
  // Pad image to avoid open mesh surfaces
  //

  typename TImage::SizeType padding;
  padding.Fill( 1 );

  const auto pad = TPad::New();
  pad->SetInput( reader->GetOutput() );
  pad->SetPadUpperBound( padding );
  pad->SetPadLowerBound( padding );
  pad->SetConstant( static_cast< TPixel >( 0 ) );

  //
  // Extract pixels in the set
  //

  const auto filter = TExtract::New();
  filter->SetLabels( LabelSet );
  filter->SetInput( pad->GetOutput() );

  //
  // Keep Connected Components
  //

  const auto connected = TLabel::New ();
  connected->SetInput( filter->GetOutput() );
  connected->FullyConnectedOn();
  connected->Update();
 
  const auto keep = TKeep::New();
  keep->SetInput( connected->GetOutput() );
  keep->SetBackgroundValue( 0 );
  keep->SetNumberOfObjects( 1 );
  keep->SetAttribute( TKeep::LabelObjectType::NUMBER_OF_PIXELS);

  const auto thresh = TThresh::New();
  thresh->SetInput( keep->GetOutput() );
  thresh->SetInsideValue( 1 );
  thresh->SetOutsideValue( 0 );
  thresh->SetLowerThreshold( 1 );

  //
  // Fill holes
  //

  const auto fill = TFill::New();
  fill->SetInput( thresh->GetOutput() );
  fill->SetForegroundValue( static_cast<TPixel>(1) );

  //
  // Meshing
  //

  const auto meshSource = TMeshSource::New();
  const TPixel objectValue = static_cast<TPixel>( 1 );
  meshSource->SetObjectValue( objectValue );
  meshSource->SetInput( fill->GetOutput() );

  //
  // Writer
  //

  const auto writer = TWriter::New();
  writer->SetInput( meshSource->GetOutput() );
  writer->SetFileName( OMesh );
  writer->Update();

}

}

#endif
