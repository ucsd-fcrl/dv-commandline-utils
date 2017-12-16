#ifndef dv_ExtractConnectedComponents_h
#define dv_ExtractConnectedComponents_h

#include <itkImageFileReader.h>
#include <itkConnectedComponentImageFilter.h>
#include <itkLabelShapeKeepNObjectsImageFilter.h>
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
  using TLabel  = itk::ConnectedComponentImageFilter<TImage, TImage>;
  using TKeep   = itk::LabelShapeKeepNObjectsImageFilter< TImage >;
  using TWriter = itk::ImageFileWriter< TImage >;

  const auto reader = TReader::New();
  reader->SetFileName( IImage );
 
  const auto connected = TLabel::New ();
  connected->SetInput(reader->GetOutput());
  connected->Update();
 
  const auto keep = TKeep::New();
  keep->SetInput( connected->GetOutput() );
  keep->SetBackgroundValue( 0 );
  keep->SetNumberOfObjects( 1 );
  keep->SetAttribute( TKeep::LabelObjectType::NUMBER_OF_PIXELS);

  const auto writer = TWriter::New();
  writer->SetInput( keep->GetOutput() );
  writer->SetFileName( OImage );
  writer->Update();

}

}

#endif
