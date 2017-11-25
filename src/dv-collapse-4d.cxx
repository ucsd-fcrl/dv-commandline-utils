// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

#include "includes/dvCollapse4D.h"

const unsigned int Dimension = 3;
using TPixel = unsigned short;

int
main(int argc, char **argv)
{

  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-image",           po::value<std::string>()->required(), "Filename of the input image.")
    ("dimension-to-collapse", po::value<unsigned int>()->required(), "Dimension to collapse.")
    ("output-image",          po::value<std::string>()->required(), "Filename of the output image.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const auto IImage        = vm["input-image"].as<std::string>();
  const auto DimToCollapse = vm["dimension-to-collapse"].as<unsigned int>();
  const auto OImage        = vm["output-image"].as<std::string>();

  if (DimToCollapse > 3)
    {
    std::cerr << "Invalid dimension supplied: " << DimToCollapse << std::endl;
    return EXIT_FAILURE;
    }

  dv::Collapse4D<Dimension, TPixel>(IImage, DimToCollapse, OImage);

  return EXIT_SUCCESS;

}

