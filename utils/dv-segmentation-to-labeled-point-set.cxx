// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Custom
#include <dvSegmentationToLabeledPointSet.h>

const unsigned int Dimension = 3;
using TPixel = unsigned char;

int
main(int argc, char** argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-segmentation", po::value<std::string>()->required(), "Filename of the input segmentation.")
    ("labeled-point-set", po::value<std::string>()->required(), "Filename of the output labeled points.");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc) {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
  }

  po::notify(vm);

  const auto input_segmentation = vm["input-segmentation"].as<std::string>();
  const auto labeled_point_set = vm["labeled-point-set"].as<std::string>();

  dv::SegmentationToLabeledPointSet<3, unsigned int, double>( input_segmentation, labeled_point_set );

  return EXIT_SUCCESS;

}
