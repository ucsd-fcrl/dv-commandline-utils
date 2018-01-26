// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Custom
#include <dvReadImageIOBase.h>
#include <dvResampleFromReference.h>

int
main( int argc, char* argv[] )
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help",           "Print usage information.")
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

  const std::string IImage(vm["input-image"].as<std::string>());
  const std::string RImage(vm["reference-image"].as<std::string>());
  const std::string OImage(vm["output-image"].as<std::string>());

  const double OutsideValue = 0.0;

  switch (dv::ReadImageIOBase(IImage)->GetComponentType())
    {
    case itk::ImageIOBase::UCHAR:
      dv::ResampleFromReference<3, unsigned char>(IImage, RImage, OImage, OutsideValue);
      break;
    case itk::ImageIOBase::CHAR:
      dv::ResampleFromReference<3, char>(IImage, RImage, OImage, OutsideValue);
      break;
    case itk::ImageIOBase::USHORT:
      dv::ResampleFromReference<3, unsigned short>(IImage, RImage, OImage, OutsideValue);
      break;
    case itk::ImageIOBase::SHORT:
      dv::ResampleFromReference<3, short>(IImage, RImage, OImage, OutsideValue);
      break;
    case itk::ImageIOBase::UINT:
      dv::ResampleFromReference<3, unsigned int>(IImage, RImage, OImage, OutsideValue);
      break;
    case itk::ImageIOBase::INT:
      dv::ResampleFromReference<3, int>(IImage, RImage, OImage, OutsideValue);
      break;
    case itk::ImageIOBase::ULONG:
      dv::ResampleFromReference<3, unsigned long>(IImage, RImage, OImage, OutsideValue);
      break;
    case itk::ImageIOBase::LONG:
      dv::ResampleFromReference<3, long>(IImage, RImage, OImage, OutsideValue);
      break;
    case itk::ImageIOBase::FLOAT:
      dv::ResampleFromReference<3, float>(IImage, RImage, OImage, OutsideValue);
      break;
    case itk::ImageIOBase::DOUBLE:
      dv::ResampleFromReference<3, double>(IImage, RImage, OImage, OutsideValue);
      break;
    default:
      std::cerr << "ERROR: Unrecognized pixel type." << std::endl;
      return EXIT_FAILURE;
      break;
    }

  return EXIT_SUCCESS;

}

