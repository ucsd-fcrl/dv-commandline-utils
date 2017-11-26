// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkFixedArray.h>

// Custom
#include "includes/dvFlipAlongAxes3D.h"

const unsigned int Dimension = 3;
using TPixel = short;
using TArray = itk::FixedArray<unsigned int, Dimension>;

int
main(int argc, char ** argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-image",  po::value<std::string>()->required(),                             "Filename of the input mesh.")
    ("output-image", po::value<std::string>()->required(),                             "Filename of the reference image.")
    ("order",        po::value<std::vector<unsigned int>>()->multitoken()->required(), "Filename of the output image.")
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
  const std::vector<unsigned int> orderVec = vm["order"].as<std::vector<unsigned int>>();

  TArray order;
  for (unsigned int i = 0; i < Dimension; ++i)
    {
    order[i] = orderVec.at(i);
    }

  dv::FlipAlongAxes3D<Dimension, TPixel>(IImage, OImage, order);

  return EXIT_SUCCESS;
}
