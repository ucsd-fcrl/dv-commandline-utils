// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkImageFileReader.h>
#include <itkPermuteAxesImageFilter.h>
#include <itkImageFileWriter.h>

const unsigned int Dimension = 3;
 
typedef itk::Image<unsigned short, Dimension> TImage;

typedef itk::ImageFileReader<TImage> TReader;
typedef itk::PermuteAxesImageFilter<TImage> TPermute;
typedef itk::ImageFileWriter<TImage> TWriter;

int
main(int argc, char ** argv)
{

  if (argc != 6)
    {
    std::cerr << "Usage: "
              << argv[0]
              << " <InputImage>"
              << " <OutputImage>"
              << " <DimX> <DimY> <DimZ>"
              << std::endl;
    return EXIT_FAILURE;
    }

  const std::string IImage = argv[1];
  const std::string OImage = argv[2];

  itk::FixedArray<unsigned int, Dimension> order;
  order[0] = std::atoi(argv[3]);
  order[1] = std::atoi(argv[4]);
  order[2] = std::atoi(argv[5]);

  const auto reader = TReader::New();
  reader->SetFileName( IImage );
  
  const auto permute = TPermute::New();
  permute->SetInput( reader->GetOutput() );
  permute->SetOrder(order);

  const auto writer = TWriter::New();
  writer->SetInput( permute->GetOutput() );
  writer->SetFileName( OImage );
  writer->Update();
 
  return EXIT_SUCCESS;
}
