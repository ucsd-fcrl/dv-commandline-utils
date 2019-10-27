// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// DV
#include <dvGenerateInitialModel.h>

int
main(int argc, char** argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()("help", "Print usage information.")(
    "input-mesh",
    po::value<std::string>()->required(),
    "Filename of the input mesh.")("output-mesh",
                                   po::value<std::string>()->required(),
                                   "Filename of the output mesh.")(
    "count",
    po::value<unsigned int>()->default_value(500),
    "Target number of cells in the decimated mesh.")(
    "sigma",
    po::value<double>()->default_value(0.1),
    "Amount of gaussian noise to add to mesh vertices prior to decimation.");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc) {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
  }

  po::notify(vm);

  const std::string inputMeshName(vm["input-mesh"].as<std::string>());
  const std::string outputMeshName(vm["output-mesh"].as<std::string>());
  const unsigned int count = vm["count"].as<unsigned int>();
  const double sigma = vm["sigma"].as<double>();

  // => Connected
  // => Delete Unassigned
  // => Noise
  // => Decimate
  // => Connected
  // => Delete Unassigned
  // => Refine
  // => Loop

  dv::GenerateInitialModel(inputMeshName, outputMeshName, count, sigma);

  return EXIT_SUCCESS;
}
