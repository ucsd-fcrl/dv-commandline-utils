// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkImageFileReader.h>
#include <itkChangeInformationImageFilter.h>
#include <itkImageFileWriter.h>

const unsigned int Dimension = 3;
 
typedef itk::Image<unsigned short, Dimension> TImage;

typedef itk::ImageFileReader<TImage> TReader;
typedef itk::ChangeInformationImageFilter<TImage> TInfo;
typedef itk::ImageFileWriter<TImage> TWriter;

int
main(int argc, char ** argv)
{

  if (argc != 4)
    {
    std::cerr << "Usage: "
              << argv[0]
              << " <InputImage>"
              << " <ReferenceImage>"
              << " <OutputImage>"
              << std::endl;
    return EXIT_FAILURE;
    }

  const std::string IImage = argv[1];
  const std::string RImage = argv[2];
  const std::string OImage = argv[3];

  const auto referenceReader = TReader::New();
  referenceReader->SetFileName( RImage );
  referenceReader->Update();

  const auto reader = TReader::New();
  reader->SetFileName( IImage );

  const auto info = TInfo::New();
  info->SetInput( reader->GetOutput() );
  info->SetReferenceImage( referenceReader->GetOutput() );
  info->UseReferenceImageOn();
  info->ChangeAll();

  const auto writer = TWriter::New();
  writer->SetInput( info->GetOutput() );
  writer->SetFileName( OImage );
  writer->Update();
 
  return EXIT_SUCCESS;
}
