// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Custom
#include <dvConvertMesh.h>

const unsigned int Dimension = 3;
using TCoordinate = float;

int main( int argc, char* argv[] )
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-mesh",  po::value<std::string>()->required(), "Filename of the input mesh.")
    ("output-mesh", po::value<std::string>()->required(), "Filename of the output mesh.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || vm.empty())
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const std::string IMesh(vm["input-mesh"].as<std::string>());
  const std::string OMesh(vm["output-mesh"].as<std::string>());

  dv::ConvertMesh<Dimension, TCoordinate>(IMesh, OMesh);

  return EXIT_SUCCESS;

}

