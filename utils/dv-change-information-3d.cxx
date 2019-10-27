// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

#include <dvChangeInformation3D.h>
#include <dvReadImageIOBase.h>

int
main(int argc, char** argv)
{

  po::options_description description("Allowed options");
  description.add_options()("help", "Print usage information.")(
    "input-image",
    po::value<std::string>()->required(),
    "Filename of the input image.")("reference-image",
                                    po::value<std::string>()->required(),
                                    "Filename of the reference image.")(
    "output-image",
    po::value<std::string>()->required(),
    "Filename of the output image.");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc) {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
  }

  po::notify(vm);

  const auto IImage = vm["input-image"].as<std::string>();
  const auto RImage = vm["reference-image"].as<std::string>();
  const auto OImage = vm["output-image"].as<std::string>();

  switch (dv::ReadImageIOBase(IImage)->GetComponentType()) {
    case itk::ImageIOBase::UCHAR:
      dv::ChangeInformation3D<3, unsigned char>(IImage, RImage, OImage);
      break;
    case itk::ImageIOBase::CHAR:
      dv::ChangeInformation3D<3, char>(IImage, RImage, OImage);
      break;
    case itk::ImageIOBase::USHORT:
      dv::ChangeInformation3D<3, unsigned short>(IImage, RImage, OImage);
      break;
    case itk::ImageIOBase::SHORT:
      dv::ChangeInformation3D<3, short>(IImage, RImage, OImage);
      break;
    case itk::ImageIOBase::UINT:
      dv::ChangeInformation3D<3, unsigned int>(IImage, RImage, OImage);
      break;
    case itk::ImageIOBase::INT:
      dv::ChangeInformation3D<3, int>(IImage, RImage, OImage);
      break;
    case itk::ImageIOBase::ULONG:
      dv::ChangeInformation3D<3, unsigned long>(IImage, RImage, OImage);
      break;
    case itk::ImageIOBase::LONG:
      dv::ChangeInformation3D<3, long>(IImage, RImage, OImage);
      break;
    case itk::ImageIOBase::FLOAT:
      dv::ChangeInformation3D<3, float>(IImage, RImage, OImage);
      break;
    case itk::ImageIOBase::DOUBLE:
      dv::ChangeInformation3D<3, double>(IImage, RImage, OImage);
      break;
    default:
      std::cerr << "ERROR: Unrecognized pixel type." << std::endl;
      return EXIT_FAILURE;
      break;
  }

  return EXIT_SUCCESS;
}
