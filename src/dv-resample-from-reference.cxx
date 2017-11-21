// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkImage.h>
#include <itkIdentityTransform.h>
#include <itkResampleImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

template<unsigned int Dimension, typename PixelType>
void
resample( const std::string iImageFileName,
          const std::string rImageFileName,
          const std::string oImageFileName,
          const double outsideValue)
{

  typedef itk::Image< PixelType, Dimension >               ImageType;
  typedef itk::ImageFileReader< ImageType >                ReaderType;
  typedef itk::ImageFileWriter< ImageType >                WriterType;
  typedef itk::IdentityTransform< double, Dimension >      TransformType;
  typedef itk::ResampleImageFilter< ImageType, ImageType > ResampleType;


  const auto iReader = ReaderType::New();
  const auto rReader = ReaderType::New();
  const auto resample = ResampleType::New();
  const auto writer = WriterType::New();

  iReader->SetFileName( iImageFileName );
  rReader->SetFileName( rImageFileName );

  resample->SetInput( iReader->GetOutput() );
  resample->SetReferenceImage( rReader->GetOutput() );
  resample->UseReferenceImageOn();
  resample->SetTransform( TransformType::New() );
  resample->SetDefaultPixelValue( outsideValue );

  writer->SetInput( resample->GetOutput() );
  writer->SetFileName( oImageFileName );
  writer->Update();


}

int
main( int argc, char* argv[] )
{

  // Deal with commandline arguments
  if (4 > argc)
    {
    std::cerr << "Usage:\n"
              << argv[0] << " <inputImage> <referenceImage> <outputImage>" << std::endl;
    return EXIT_FAILURE;
    }

  const std::string iImageFileName = argv[1];
  const std::string rImageFileName = argv[2];
  const std::string oImageFileName = argv[3];
  const double outsideValue = (argc > 4) ? std::atof(argv[4]) : 0.0;

  resample< 3, signed short>(iImageFileName, rImageFileName, oImageFileName, outsideValue);

  return EXIT_SUCCESS;

}

