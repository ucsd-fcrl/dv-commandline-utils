// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Custom
#include <dvAnisotropicSmoothing.h>
#include <dvReadImageIOBase.h>

int
main(int argc, char** argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()("help", "Print usage information.")(
    "input-image",
    po::value<std::string>()->required(),
    "Filename of the input image.")("output-image",
                                    po::value<std::string>()->required(),
                                    "Filename of the output image.")(
    "iterations", po::value<unsigned int>()->default_value(5), "iterations")(
    "time-step", po::value<double>()->default_value(0.125), "time step")(
    "conductance", po::value<double>()->default_value(3.0), "conductance");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc) {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
  }

  po::notify(vm);

  const auto IImage = vm["input-image"].as<std::string>();
  const auto OImage = vm["output-image"].as<std::string>();
  const auto Iterations = vm["iterations"].as<unsigned int>();
  const auto TimeStep = vm["time-step"].as<double>();
  const auto Conductance = vm["conductance"].as<double>();

  //
  // make Set from labels given
  //

  switch (dv::ReadImageIOBase(IImage)->GetComponentType()) {
    case itk::ImageIOBase::FLOAT:
      dv::AnisotropicSmoothing<3, float>(
        IImage, OImage, Iterations, TimeStep, Conductance);
      break;
    case itk::ImageIOBase::DOUBLE:
      dv::AnisotropicSmoothing<3, double>(
        IImage, OImage, Iterations, TimeStep, Conductance);
      break;
    default:
      std::cerr
        << "WARNING: Pixel type is not floating point; casting to double."
        << std::endl;
      dv::AnisotropicSmoothing<3, double>(
        IImage, OImage, Iterations, TimeStep, Conductance);
      return EXIT_FAILURE;
      break;
  }

  return EXIT_SUCCESS;
}
