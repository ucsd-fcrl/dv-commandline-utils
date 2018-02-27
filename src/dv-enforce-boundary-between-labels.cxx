// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Custom
#include <dvEnforceBoundaryBetweenLabels.h>

// STD
#include <vector>

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
    ("input-image", po::value<std::string>()->required(),       "Filename of the segmentation image.")
    ("output-image", po::value<std::string>()->required(),      "Filename of the output image.")
    ("labels-1", po::value<std::vector<short>>()->multitoken(), "First label set to extract.")
    ("labels-2", po::value<std::vector<short>>()->multitoken(), "Second label set to extract.")
    ("radius-1", po::value<short>()->default_value(1), "Dilation radius for set 1.")
    ("radius-2", po::value<short>()->default_value(1), "Dilation radius for set 2.")
    ("replacement", po::value<short>()->default_value(0), "Dilation radius for set 2.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const std::string IImage(vm["input-image"].as<std::string>());
  const std::string OImage(vm["output-image"].as<std::string>());
  const std::vector<short> labels_vector_1 = vm["labels-1"].as<std::vector<short>>();
  const std::vector<short> labels_vector_2 = vm["labels-2"].as<std::vector<short>>();
  const short radius_1 = vm["radius-1"].as<short>();
  const short radius_2 = vm["radius-2"].as<short>();
  const short replacement = vm["replacement"].as<short>();

  const std::set<TPixel> labels_set_1(labels_vector_1.begin(), labels_vector_1.end());
  const std::set<TPixel> labels_set_2(labels_vector_2.begin(), labels_vector_2.end());

  //
  // make Set from labels given
  //
  dv::EnforceBoundaryBetweenLabels<Dimension, TPixel, TCoordinate>(IImage, OImage, labels_set_1, labels_set_2, radius_1, radius_2, replacement);

  return EXIT_SUCCESS;

}
