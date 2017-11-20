// Boost
#include <boost/program_options.hpp>

// ITK
#include <itkImage.h>
#include <itkIdentityTransform.h>
#include <itkResampleImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkFileTools.h>

// Typedefs
const unsigned int Dimension = 3;

typedef signed short                                     PixelType;
typedef itk::Image< PixelType, Dimension >               ImageType;
typedef itk::ImageFileReader< ImageType >                ReaderType;
typedef itk::ImageFileWriter< ImageType >                WriterType;
typedef itk::IdentityTransform< double, Dimension >      TransformType;
typedef itk::ResampleImageFilter< ImageType, ImageType > ResampleType;

int
main( int argc, char* argv[] )
{

  // Deal with commandline arguments
  if (4 != argc)
    {
    std::cerr << "Usage:\n"
              << argv[0] << " <inputImage> <outputImage> <spacing>" << std::endl;
    return EXIT_FAILURE;
    }

  const std::string inputImage = argv[1];
  const std::string outputImage = argv[2];
  const double spacing = std::atof(argv[3]);

  const auto reader = ReaderType::New();
  const auto resample = ResampleType::New();
  const auto writer = WriterType::New();

  reader->SetFileName( inputImage );
  reader->Update();

  const auto image = ImageType::New();
  image->Graft( reader->GetOutput() );

  ImageType::SpacingType outputSpacing;
  outputSpacing.Fill( spacing );
  auto inputSize = image->GetLargestPossibleRegion().GetSize();
  auto inputSpacing = image->GetSpacing();
  ImageType::SizeType outputSize;
  for (unsigned int i = 0; i < Dimension; ++i)
    {
    outputSize[i] = std::floor(double(inputSize[i]) * inputSpacing[i] / outputSpacing[i]);
    }

  resample->SetTransform( TransformType::New() );
  resample->SetOutputStartIndex( image->GetLargestPossibleRegion().GetIndex() );
  resample->SetOutputOrigin( image->GetOrigin() );
  resample->SetOutputDirection( image->GetDirection() );
  resample->SetOutputSpacing( outputSpacing );
  resample->SetSize( outputSize );
  resample->SetInput( image );

  writer->SetInput( resample->GetOutput() );
  writer->SetFileName( outputImage );
  writer->Update();

  return EXIT_SUCCESS;

}

