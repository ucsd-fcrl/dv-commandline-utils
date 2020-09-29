// Boost
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

namespace po = boost::program_options;

#include <dvExplode4D.h>
#include <dvReadImageIOBase.h>

int
main(int argc, char** argv)
{

  po::options_description description("Allowed options");
  description.add_options()("help", "Print usage information.")
    ("input-image",
     po::value<std::string>()->required(),
     "Filename of the input image.")
    ("dimension-to-explode",
     po::value<unsigned int>()->default_value(3),
     "Dimension to explode.")
    ("output-directory",
     po::value<boost::filesystem::path>()->required(),
     "Name of the output directory.");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc) {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
  }

  po::notify(vm);

  const auto IImage = vm["input-image"].as<std::string>();
  const auto DimToExplode = vm["dimension-to-explode"].as<unsigned int>();
  const auto ODir = vm["output-directory"].as<boost::filesystem::path>();

  if (DimToExplode > 3) {
    std::cerr << "Invalid dimension supplied: " << DimToExplode << std::endl;
    return EXIT_FAILURE;
  }

  if (!boost::filesystem::create_directories(boost::filesystem::path(ODir))) {
      std::cerr << "The output directory " << ODir << " could not be created.\n";
      return EXIT_FAILURE;
  }

  switch (dv::ReadImageIOBase(IImage)->GetComponentType()) {
    case itk::ImageIOBase::UCHAR:
      dv::Explode4D<3, unsigned char>(IImage, DimToExplode, ODir);
      break;
    case itk::ImageIOBase::CHAR:
      dv::Explode4D<3, char>(IImage, DimToExplode, ODir);
      break;
    case itk::ImageIOBase::USHORT:
      dv::Explode4D<3, unsigned short>(IImage, DimToExplode, ODir);
      break;
    case itk::ImageIOBase::SHORT:
      dv::Explode4D<3, short>(IImage, DimToExplode, ODir);
      break;
    case itk::ImageIOBase::UINT:
      dv::Explode4D<3, unsigned int>(IImage, DimToExplode, ODir);
      break;
    case itk::ImageIOBase::INT:
      dv::Explode4D<3, int>(IImage, DimToExplode, ODir);
      break;
    case itk::ImageIOBase::ULONG:
      dv::Explode4D<3, unsigned long>(IImage, DimToExplode, ODir);
      break;
    case itk::ImageIOBase::LONG:
      dv::Explode4D<3, long>(IImage, DimToExplode, ODir);
      break;
    case itk::ImageIOBase::FLOAT:
      dv::Explode4D<3, float>(IImage, DimToExplode, ODir);
      break;
    case itk::ImageIOBase::DOUBLE:
      dv::Explode4D<3, double>(IImage, DimToExplode, ODir);
      break;
    default:
      std::cerr << "ERROR: Unrecognized pixel type." << std::endl;
      return EXIT_FAILURE;
      break;
  }

  return EXIT_SUCCESS;
}
