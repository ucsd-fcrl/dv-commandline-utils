// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

#include <dvChangeInformation3D.h>

int
main(int argc, char ** argv)
{

  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-image",     po::value<std::string>()->required(), "Filename of the input image.")
    ("reference-image", po::value<std::string>()->required(), "Filename of the reference image.")
    ("output-image",    po::value<std::string>()->required(), "Filename of the output image.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const auto IImage = vm["input-image"].as<std::string>();
  const auto RImage = vm["reference-image"].as<std::string>();
  const auto OImage = vm["output-image"].as<std::string>();

  dv::ChangeInformation3D<Dimension, TPixel>(IImage, RImage, OImage);
 
  return EXIT_SUCCESS;
}
