#ifndef dv_ExtractConnectedComponents_h
#define dv_ExtractConnectedComponents_h

#include <itkImageFileReader.h>
#include <itkConnectedComponentImageFilter.h>
#include <itkLabelShapeKeepNObjectsImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkImageFileWriter.h>
#include <itkMaskImageFilter.h>

#include <dvImageToSet.h>

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
  using TMask   = itk::MaskImageFilter< TImage, TImage >;

  auto input = TImage::New();

    {
    const auto reader = TReader::New();
    reader->SetFileName( IImage );
    reader->Update();
    input->Graft( reader->GetOutput() );
    }

  const auto labels_set = dv::ImageToSet<Dimension, TPixel>( input );

  for (const auto &s : labels_set)
    {
    // Ignore background.
    if (0 == s) continue;

    // Extract Label:
    const auto thresh = TThresh::New();
    thresh->SetInput( input );
    thresh->SetLowerThreshold( s );
    thresh->SetUpperThreshold( s );
    thresh->SetInsideValue( s );
    thresh->SetOutsideValue( 0 );

    // Connected Components:
    const auto connected = TLabel::New ();
    connected->SetInput( thresh->GetOutput() );
    connected->FullyConnectedOn();
    connected->Update();

    const auto NumObjects = connected->GetObjectCount();

    // Continue if only 1 object is detected.
    if (NumObjects <= N) continue;

    // Keep (ObjectCount - 1) smallest components.
    const auto keep = TKeep::New();
    keep->SetInput( connected->GetOutput() );
    keep->SetBackgroundValue( 0 );
    keep->SetNumberOfObjects( NumObjects - N );
    keep->SetAttribute( TKeep::LabelObjectType::NUMBER_OF_PIXELS);
    keep->ReverseOrderingOn();

    // Turn it into a binary
    const auto thresh2 = TThresh::New();
    thresh2->SetInput( keep->GetOutput() );
    thresh2->SetLowerThreshold( 1 );
    thresh2->SetInsideValue( 1 );
    thresh2->SetOutsideValue( 0 );

    // Mask the input
    const auto mask = TMask::New();
    mask->SetInput( input );
    mask->SetMaskImage( thresh2->GetOutput() );
    mask->SetMaskingValue( 1 );
    mask->Update();

    // Replace the input
    input->Graft( mask->GetOutput() );
    input->DisconnectPipeline();
    }

  const auto writer = TWriter::New();
  writer->SetInput( input );
  writer->SetFileName( OImage );
  writer->Update();

}

}

#endif
