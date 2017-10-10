// Boost
#include <boost/program_options.hpp>

// ITK
#include <itkImageFileReader.h>
#include <itkExtractImageFilter.h>
#include <itkImageFileWriter.h>

typedef unsigned short TIOPixel;

const unsigned int Dimension = 3;

typedef itk::Image< TIOPixel, Dimension >     TImage3D;
typedef itk::Image< TIOPixel, Dimension + 1 > TImage4D;

typedef itk::ImageFileReader< TImage4D >              TReader;
typedef itk::ExtractImageFilter< TImage4D, TImage3D > TExtract;
typedef itk::ImageFileWriter< TImage3D >              TWriter;

int
main(int argc, char **argv)
{

  ////////////////
  // Parameters //
  ////////////////

  if (4 != argc)
    {
    std::cerr << "Usage: ";
    std::cerr << argv[0] << " <4D Image File>"
                         << " <3D Image File>"
                         << " <Dimension to Collapse>"
                         << std::endl;
    return EXIT_FAILURE;
    }

  const std::string Image4D = argv[1];
  const std::string Image3D = argv[2];
  const unsigned int DimToCollapse = std::atoi(argv[3]);

  if (DimToCollapse > 3)
    {
    std::cerr << "Invalid dimension supplied: " << DimToCollapse << std::endl;
    return EXIT_FAILURE;
    }

  const auto reader = TReader::New();
  reader->SetFileName( Image4D );
  reader->Update();

  auto region = reader->GetOutput()->GetLargestPossibleRegion();

  region.GetModifiableSize()[DimToCollapse] = 0;

  const auto extract = TExtract::New();
  extract->SetInput( reader->GetOutput() );
  extract->SetExtractionRegion( region );
  extract->SetDirectionCollapseToSubmatrix();

  const auto writer = TWriter::New();
  writer->SetInput( extract->GetOutput() );
  writer->SetFileName( Image3D );
  writer->Update();

  return EXIT_SUCCESS;

}

