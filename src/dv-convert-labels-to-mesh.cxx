// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Custom
#include <dvConvertLabelsToMesh.h>

const unsigned int Dimension = 3;
using TCoordinate = float;
using TPixel = unsigned char;

int
main( int argc, char* argv[] )
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-image", po::value<std::string>()->required(),          "Filename of the input mesh.")
    ("output-mesh", po::value<std::string>()->required(),          "Filename of the output image.")
    ("labels",      po::value<std::vector<short>>()->multitoken(), "Labels to extract from input image.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || vm.empty())
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const std::string inputFileName(vm["input-image"].as<std::string>());
  const std::string outputFileName(vm["output-mesh"].as<std::string>());
  const std::vector<short> labelVector = vm["labels"].as<std::vector<short>>();

  //
  // make Set from labels given
  //
  std::set<TPixel> labels(labelVector.cbegin(), labelVector.cend());

  dv::ConvertLabelsToMesh<Dimension, TPixel, TCoordinate>(inputFileName, labels, outputFileName);

  return EXIT_SUCCESS;

}
