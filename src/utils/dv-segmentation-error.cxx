// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Custom
#include <dvReadImageIOBase.h>
#include <dvSegmentationError.h>

int
main( int argc, char* argv[] )
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("source-image", po::value<std::string>()->required(), "Filename of source image.")
    ("target-image", po::value<std::string>()->required(), "Filename of target image.")
    ("label", po::value<long>()->default_value(1),         "Label.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc)
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const std::string SImage = vm["source-image"].as<std::string>();
  const std::string TImage = vm["target-image"].as<std::string>();
  const long Label = vm["label"].as<long>();

  switch (dv::ReadImageIOBase(SImage)->GetComponentType())
    {
    case itk::ImageIOBase::UCHAR:
      dv::SegmentationError<3, unsigned char>(SImage, TImage, Label);
      break;
    case itk::ImageIOBase::CHAR:
      dv::SegmentationError<3, char>(SImage, TImage, Label);
      break;
    case itk::ImageIOBase::USHORT:
      dv::SegmentationError<3, unsigned short>(SImage, TImage, Label);
      break;
    case itk::ImageIOBase::SHORT:
      dv::SegmentationError<3, short>(SImage, TImage, Label);
      break;
    case itk::ImageIOBase::UINT:
      dv::SegmentationError<3, unsigned int>(SImage, TImage, Label);
      break;
    case itk::ImageIOBase::INT:
      dv::SegmentationError<3, int>(SImage, TImage, Label);
      break;
    case itk::ImageIOBase::ULONG:
      dv::SegmentationError<3, unsigned long>(SImage, TImage, Label);
      break;
    case itk::ImageIOBase::LONG:
      dv::SegmentationError<3, long>(SImage, TImage, Label);
      break;
    default:
      std::cerr << "ERROR: Unrecognized pixel type." << std::endl;
      return EXIT_FAILURE;
      break;
    }

  return EXIT_SUCCESS;

}

