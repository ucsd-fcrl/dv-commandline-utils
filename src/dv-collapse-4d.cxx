// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkImageFileReader.h>
#include <itkExtractImageFilter.h>
#include <itkImageFileWriter.h>

using TIOPixel = unsigned short;

const unsigned int Dimension = 3;

using TImage3D = itk::Image< TIOPixel, Dimension >;
using TImage4D = itk::Image< TIOPixel, Dimension + 1 >;
using TReader  = itk::ImageFileReader< TImage4D >;
using TExtract = itk::ExtractImageFilter< TImage4D, TImage3D >;
using TWriter  =itk::ImageFileWriter< TImage3D >;

int
main(int argc, char **argv)
{

  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-image",           po::value<std::string>()->required(), "Filename of the input image.")
    ("output-image",          po::value<std::string>()->required(), "Filename of the output image.")
    ("dimension-to-collapse", po::value<unsigned int>()->required(), "Dimension to collapse.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

//  const std::string Image4D = argv[1];
//  const std::string Image3D = argv[2];
//  const unsigned int DimToCollapse = std::atoi(argv[3]);

  if (vm["dimension-to-collapse"].as<unsigned int>() > 3)
    {
    std::cerr << "Invalid dimension supplied: " << vm["dimension-to-collapse"].as<unsigned int>() << std::endl;
    return EXIT_FAILURE;
    }

  const auto reader = TReader::New();
  reader->SetFileName( vm["input-image"].as<std::string>() );
  reader->Update();

  auto region = reader->GetOutput()->GetLargestPossibleRegion();

  region.GetModifiableSize()[vm["dimension-to-collapse"].as<unsigned int>()] = 0;

  const auto extract = TExtract::New();
  extract->SetInput( reader->GetOutput() );
  extract->SetExtractionRegion( region );
  extract->SetDirectionCollapseToSubmatrix();

  const auto writer = TWriter::New();
  writer->SetInput( extract->GetOutput() );
  writer->SetFileName( vm["output-image"].as<std::string>() );
  writer->Update();

  return EXIT_SUCCESS;

}

