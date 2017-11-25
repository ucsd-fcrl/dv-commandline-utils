// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

#include "includes/dvConvertImage.h"

const unsigned int Dimension = 3;
using TPixel = short;

int
main(int argc, char **argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-image", po::value<std::string>()->required(), "Filename of input image.")
    ("output-image", po::value<std::string>()->required(), "Filename of output image.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const std::string IImage = vm["input-image"].as<std::string>();
  const std::string OImage = vm["output-image"].as<std::string>();

  dv::ConvertImage<Dimension, TPixel>(IImage, OImage);

  return EXIT_SUCCESS;
}
