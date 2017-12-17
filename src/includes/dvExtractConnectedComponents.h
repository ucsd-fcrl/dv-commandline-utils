#ifndef dv_ExtractConnectedComponents_h
#define dv_ExtractConnectedComponents_h

#include <itkImageFileReader.h>
#include <itkConnectedComponentImageFilter.h>
#include <itkLabelShapeKeepNObjectsImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkImageFileWriter.h>

namespace dv
{

template<unsigned int Dimension, typename TPixel>
void
ExtractConnectedComponents(const std::string &IImage,
                           const std::string &OImage,
                           const unsigned int N)
{

  using TImage  = itk::Image< TPixel, Dimension >;
  using TReader = itk::ImageFileReader< TImage >;
  using TLabel  = itk::ConnectedComponentImageFilter< TImage, TImage >;
  using TKeep   = itk::LabelShapeKeepNObjectsImageFilter< TImage >;
  using TThresh = itk::BinaryThresholdImageFilter< TImage, TImage >;
  using TWriter = itk::ImageFileWriter< TImage >;

  const auto reader = TReader::New();
  reader->SetFileName( IImage );
 
  const auto connected = TLabel::New ();
  connected->SetInput(reader->GetOutput());
  connected->FullyConnectedOn();
  connected->Update();
 
  const auto keep = TKeep::New();
  keep->SetInput( connected->GetOutput() );
  keep->SetBackgroundValue( 0 );
  keep->SetNumberOfObjects( N );
  keep->SetAttribute( TKeep::LabelObjectType::NUMBER_OF_PIXELS);

  const auto thresh = TThresh::New();
  thresh->SetInput( keep->GetOutput() );
  thresh->SetInsideValue( 1 );
  thresh->SetOutsideValue( 0 );
  thresh->SetLowerThreshold( 1 );

  const auto writer = TWriter::New();
  writer->SetInput( thresh->GetOutput() );
  writer->SetFileName( OImage );
  writer->Update();

}

}

#endif
