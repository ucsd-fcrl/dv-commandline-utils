// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Custom
#include <dvReadImageIOBase.h>
#include <dvExtractConnectedComponents.h>
 
int
main( int argc, char **argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-image", po::value<std::string>()->required(),  "Filename of input image.")
    ("output-image", po::value<std::string>()->required(), "Filename of output image.")
    ("N", po::value<unsigned int>()->default_value( 1 ),   "Number of connected components.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc)
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const std::string IImage = vm["input-image"].as<std::string>();
  const std::string OImage = vm["output-image"].as<std::string>();
  const unsigned int N = vm["N"].as<unsigned int>();

  switch (dv::ReadImageIOBase(IImage)->GetComponentType())
    {
    case itk::ImageIOBase::UCHAR:
      dv::ExtractConnectedComponents<3, unsigned char>(IImage, OImage, N);
      break;
    case itk::ImageIOBase::CHAR:
      dv::ExtractConnectedComponents<3, char>(IImage, OImage, N);
      break;
    case itk::ImageIOBase::USHORT:
      dv::ExtractConnectedComponents<3, unsigned short>(IImage, OImage, N);
      break;
    case itk::ImageIOBase::SHORT:
      dv::ExtractConnectedComponents<3, short>(IImage, OImage, N);
      break;
    case itk::ImageIOBase::UINT:
      dv::ExtractConnectedComponents<3, unsigned int>(IImage, OImage, N);
      break;
    case itk::ImageIOBase::INT:
      dv::ExtractConnectedComponents<3, int>(IImage, OImage, N);
      break;
    case itk::ImageIOBase::ULONG:
      dv::ExtractConnectedComponents<3, unsigned long>(IImage, OImage, N);
      break;
    case itk::ImageIOBase::LONG:
      dv::ExtractConnectedComponents<3, long>(IImage, OImage, N);
      break;
    default:
      std::cerr << "ERROR: Unrecognized pixel type." << std::endl;
      return EXIT_FAILURE;
      break;
    }

  return EXIT_SUCCESS;
}
