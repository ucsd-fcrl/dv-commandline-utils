// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkSimplexMesh.h>
#include <itkMeshFileReader.h>
#include <itkSimplexMeshVolumeCalculator.h>
#include <itkSTLMeshIO.h>
#include <itksys/SystemTools.hxx>

const unsigned int Dimension = 3;
using TCoordinate = float;

using TMesh   = itk::SimplexMesh< TCoordinate, Dimension >;
using TReader = itk::MeshFileReader< TMesh >;
using TVolume = itk::SimplexMeshVolumeCalculator< TMesh >;

int
main( int argc, char* argv[] )
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-mesh",  po::value<std::string>()->required(), "Filename of the input mesh.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const std::string inputFileName(vm["input-mesh"].as<std::string>());

  const auto reader = TReader::New();
  reader->SetFileName( inputFileName );
  const auto i_ext = itksys::SystemTools::GetFilenameLastExtension(inputFileName);
  if (i_ext == ".stl" || i_ext == ".STL")
    {
    reader->SetMeshIO( itk::STLMeshIO::New() );
    }

  reader->Update();

  const auto volume = TVolume::New();
  volume->SetSimplexMesh( reader->GetOutput() );
  volume->Compute();

  std::cout << "Volume: " << volume->GetVolume() << std::endl;
  std::cout << "Area: "   << volume->GetArea() << std::endl;

  return EXIT_SUCCESS;

}

