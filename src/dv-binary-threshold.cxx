// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Custom
#include "includes/dvBinaryThreshold.h"

const unsigned int Dimension = 3;

int
main(int argc, char **argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-image", po::value<std::string>()->required(), "Filename of input image.")
    ("output-image", po::value<std::string>()->required(), "Filename of output image.")
    ("interior-value", po::value<signed short>(), "Value to set interior pixels.")
    ("exterior-value", po::value<signed short>(), "Value to set exterior pixels.")
    ("lower-threshold", po::value<signed short>(), "Lower threshold.")
    ("upper-threshold", po::value<signed short>(), "Upper threshold.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  dv::BinaryThreshold<Dimension, signed short>(vm);

  return EXIT_SUCCESS;
}