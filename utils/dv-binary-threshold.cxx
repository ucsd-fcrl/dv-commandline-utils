// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Custom
#include <dvBinaryThreshold.h>
#include <dvImageInformationFunctors.h>
#include <dvReadImageIOBase.h>

int
main(int argc, char** argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()("help", "Print usage information.")(
    "input-image",
    po::value<std::string>()->required(),
    "Filename of input image.")("output-image",
                                po::value<std::string>()->required(),
                                "Filename of output image.")(
    "interior-value", po::value<double>(), "Value to set interior pixels.")(
    "exterior-value", po::value<double>(), "Value to set exterior pixels.")(
    "lower-threshold", po::value<double>(), "Lower threshold.")(
    "upper-threshold", po::value<double>(), "Upper threshold.");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc) {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
  }

  po::notify(vm);

  const auto IImage = vm["input-image"].as<std::string>();

  switch (dv::ReadImageIOBase(IImage)->GetComponentType()) {
    case itk::ImageIOBase::UCHAR:
      dv::BinaryThreshold<3, unsigned char>(vm);
      break;
    case itk::ImageIOBase::CHAR:
      dv::BinaryThreshold<3, char>(vm);
      break;
    case itk::ImageIOBase::USHORT:
      dv::BinaryThreshold<3, unsigned short>(vm);
      break;
    case itk::ImageIOBase::SHORT:
      dv::BinaryThreshold<3, short>(vm);
      break;
    case itk::ImageIOBase::UINT:
      dv::BinaryThreshold<3, unsigned int>(vm);
      break;
    case itk::ImageIOBase::INT:
      dv::BinaryThreshold<3, int>(vm);
      break;
    case itk::ImageIOBase::ULONG:
      dv::BinaryThreshold<3, unsigned long>(vm);
      break;
    case itk::ImageIOBase::LONG:
      dv::BinaryThreshold<3, long>(vm);
      break;
    case itk::ImageIOBase::FLOAT:
      dv::BinaryThreshold<3, float>(vm);
      break;
    case itk::ImageIOBase::DOUBLE:
      dv::BinaryThreshold<3, double>(vm);
      break;
    default:
      std::cerr << "ERROR: Unrecognized pixel type." << std::endl;
      return EXIT_FAILURE;
      break;
  }

  return EXIT_SUCCESS;
}
