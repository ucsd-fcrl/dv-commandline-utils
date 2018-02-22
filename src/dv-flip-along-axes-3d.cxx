// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkFixedArray.h>

// Custom
#include <dvReadImageIOBase.h>
#include <dvFlipAlongAxes3D.h>

const unsigned int Dimension = 3;
//using TPixel = short;
using TArray = itk::FixedArray<unsigned int, Dimension>;

int
main(int argc, char ** argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-image",  po::value<std::string>()->required(),                             "Filename of the input mesh.")
    ("output-image", po::value<std::string>()->required(),                             "Filename of the output image.")
    ("order",        po::value<std::vector<unsigned int>>()->multitoken()->required(), "e.g., 1 0 0 to flip the first axis.")
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

  switch (dv::ReadImageIOBase(IImage)->GetComponentType())
    {
    case itk::ImageIOBase::UCHAR:
      dv::FlipAlongAxes3D<3, unsigned char>(IImage, OImage, order);
      break;
    case itk::ImageIOBase::CHAR:
      dv::FlipAlongAxes3D<3, char>(IImage, OImage, order);
      break;
    case itk::ImageIOBase::USHORT:
      dv::FlipAlongAxes3D<3, unsigned short>(IImage, OImage, order);
      break;
    case itk::ImageIOBase::SHORT:
      dv::FlipAlongAxes3D<3, short>(IImage, OImage, order);
      break;
    case itk::ImageIOBase::UINT:
      dv::FlipAlongAxes3D<3, unsigned int>(IImage, OImage, order);
      break;
    case itk::ImageIOBase::INT:
      dv::FlipAlongAxes3D<3, int>(IImage, OImage, order);
      break;
    case itk::ImageIOBase::ULONG:
      dv::FlipAlongAxes3D<3, unsigned long>(IImage, OImage, order);
      break;
    case itk::ImageIOBase::LONG:
      dv::FlipAlongAxes3D<3, long>(IImage, OImage, order);
      break;
    case itk::ImageIOBase::FLOAT:
      dv::FlipAlongAxes3D<3, float>(IImage, OImage, order);
      break;
    case itk::ImageIOBase::DOUBLE:
      dv::FlipAlongAxes3D<3, double>(IImage, OImage, order);
      break;
    default:
      std::cerr << "ERROR: Unrecognized pixel type." << std::endl;
      return EXIT_FAILURE;
      break;
    }

  return EXIT_SUCCESS;
}
