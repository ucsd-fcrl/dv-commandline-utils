// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Custom
#include <dvExtractInterfaceBetweenSegments.h>

// STD
#include <vector>

const unsigned int Dimension = 3;
using TCoordinate = float;
using TPixel = unsigned char;

int
main(int argc, char* argv[])
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()("help", "Print usage information.")(
    "input-image",
    po::value<std::string>()->required(),
    "Filename of the segmentation image.")("output-mesh",
                                           po::value<std::string>()->required(),
                                           "Filename of the output mesh.")(
    "labels-1",
    po::value<std::vector<short>>()->multitoken(),
    "First label set to extract.")(
    "labels-2",
    po::value<std::vector<short>>()->multitoken(),
    "Second label set to extract.");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc) {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
  }

  po::notify(vm);

  const std::string inputFileName(vm["input-image"].as<std::string>());
  const std::string outputFileName(vm["output-mesh"].as<std::string>());
  const std::vector<short> labels_vector_1 =
    vm["labels-1"].as<std::vector<short>>();
  const std::vector<short> labels_vector_2 =
    vm["labels-2"].as<std::vector<short>>();

  const std::set<TPixel> labels_set_1(labels_vector_1.begin(),
                                      labels_vector_1.end());
  const std::set<TPixel> labels_set_2(labels_vector_2.begin(),
                                      labels_vector_2.end());

  //
  // make Set from labels given
  //
  dv::ExtractInterfaceBetweenSegments<Dimension, TPixel, TCoordinate>(
    inputFileName, outputFileName, labels_set_1, labels_set_2);

  return EXIT_SUCCESS;
}
