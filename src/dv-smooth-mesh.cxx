// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Custom
#include <dvSmoothMesh.h>

const unsigned int Dimension = 3;
using ImagePixelType = unsigned char;

int main(int argc, char** argv)
{

  std::string IMesh, OMesh;
  unsigned int iterations = 1;

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-mesh",  po::value<std::string>(&IMesh)->required(), "Filename of the input mesh.")
    ("output-mesh", po::value<std::string>(&OMesh)->required(), "Filename of the output mesh.")
    ("iterations", po::value<unsigned int>(&iterations)->default_value(1), "Number of smoothing iterations.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  dv::SmoothMesh< Dimension, double >(IMesh, OMesh, iterations);

  return EXIT_SUCCESS;
}
