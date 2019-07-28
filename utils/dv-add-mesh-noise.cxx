
// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkMesh.h>
#include <itkMeshFileReader.h>
#include <itkMeshFileWriter.h>
#include <itksys/SystemTools.hxx>
#include <itkAdditiveGaussianNoiseMeshFilter.h>

const unsigned int Dimension = 3;
using TCoordinate = float;

using TMesh      = itk::Mesh< TCoordinate, Dimension >;
using TReader    = itk::MeshFileReader< TMesh >;
using TWriter    = itk::MeshFileWriter< TMesh >;
using TNoise     = itk::AdditiveGaussianNoiseMeshFilter< TMesh >;

int
main( int argc, char* argv[] )
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-mesh",  po::value<std::string>()->required(), "Filename of the input mesh.")
    ("output-mesh", po::value<std::string>()->required(), "Filename of the output image.")
    ("sigma",       po::value<double>()->required(),      "Amount of noise to be added.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc)
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const std::string inputFileName(vm["input-mesh"].as<std::string>());
  const std::string outputFileName(vm["output-mesh"].as<std::string>());

  const auto sigma = vm["sigma"].as<double>();

  const auto reader = TReader::New();
  reader->SetFileName( inputFileName );

  const auto noise = TNoise::New();
  noise->SetInput( reader->GetOutput() );
  noise->SetSigma( sigma );

  const auto writer = TWriter::New();
  writer->SetInput( noise->GetOutput() );
  writer->SetFileName( outputFileName );

  try
    {
    writer->Update();
    }
  catch ( itk::ExceptionObject & err )
    {
    std::cerr << "There was a problem writing the file." << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;

}

