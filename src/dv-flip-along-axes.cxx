// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkFixedArray.h>

// Custom
#include <dvReadImageIOBase.h>
#include <dvFlipAlongAxes.h>

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

  if (vm.count("help") || vm.empty())
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const std::string IImage = vm["input-image"].as<std::string>();
  const std::string OImage = vm["output-image"].as<std::string>();
  const std::vector<unsigned int> orderVec = vm["order"].as<std::vector<unsigned int>>();

  switch (dv::ReadImageIOBase(IImage)->GetNumberOfDimensions())
    {

    case 2:
      {
      constexpr unsigned int DIM = 2;
      using TArray = itk::FixedArray<unsigned int, DIM>;

      TArray order;
      for (unsigned int i = 0; i < DIM; ++i)
        {
        order[i] = orderVec.at(i);
        }

      switch (dv::ReadImageIOBase(IImage)->GetComponentType())
        {
        case itk::ImageIOBase::UCHAR:
          dv::FlipAlongAxes<DIM, unsigned char>(IImage, OImage, order);
          break;
        case itk::ImageIOBase::CHAR:
          dv::FlipAlongAxes<DIM, char>(IImage, OImage, order);
          break;
        case itk::ImageIOBase::USHORT:
          dv::FlipAlongAxes<DIM, unsigned short>(IImage, OImage, order);
          break;
        case itk::ImageIOBase::SHORT:
          dv::FlipAlongAxes<DIM, short>(IImage, OImage, order);
          break;
        case itk::ImageIOBase::UINT:
          dv::FlipAlongAxes<DIM, unsigned int>(IImage, OImage, order);
          break;
        case itk::ImageIOBase::INT:
          dv::FlipAlongAxes<DIM, int>(IImage, OImage, order);
          break;
        case itk::ImageIOBase::ULONG:
          dv::FlipAlongAxes<DIM, unsigned long>(IImage, OImage, order);
          break;
        case itk::ImageIOBase::LONG:
          dv::FlipAlongAxes<DIM, long>(IImage, OImage, order);
          break;
        case itk::ImageIOBase::FLOAT:
          dv::FlipAlongAxes<DIM, float>(IImage, OImage, order);
          break;
        case itk::ImageIOBase::DOUBLE:
          dv::FlipAlongAxes<DIM, double>(IImage, OImage, order);
          break;
        default:
          std::cerr << "ERROR: Unrecognized pixel type." << std::endl;
          return EXIT_FAILURE;
          break;
      }
      break;
      }
    case 3:
      {
      constexpr unsigned int DIM = 3;
      using TArray = itk::FixedArray<unsigned int, DIM>;

      TArray order;
      for (unsigned int i = 0; i < DIM; ++i)
        {
        order[i] = orderVec.at(i);
        }

      switch (dv::ReadImageIOBase(IImage)->GetComponentType())
        {
        case itk::ImageIOBase::UCHAR:
          dv::FlipAlongAxes<DIM, unsigned char>(IImage, OImage, order);
          break;
        case itk::ImageIOBase::CHAR:
          dv::FlipAlongAxes<DIM, char>(IImage, OImage, order);
          break;
        case itk::ImageIOBase::USHORT:
          dv::FlipAlongAxes<DIM, unsigned short>(IImage, OImage, order);
          break;
        case itk::ImageIOBase::SHORT:
          dv::FlipAlongAxes<DIM, short>(IImage, OImage, order);
          break;
        case itk::ImageIOBase::UINT:
          dv::FlipAlongAxes<DIM, unsigned int>(IImage, OImage, order);
          break;
        case itk::ImageIOBase::INT:
          dv::FlipAlongAxes<DIM, int>(IImage, OImage, order);
          break;
        case itk::ImageIOBase::ULONG:
          dv::FlipAlongAxes<DIM, unsigned long>(IImage, OImage, order);
          break;
        case itk::ImageIOBase::LONG:
          dv::FlipAlongAxes<DIM, long>(IImage, OImage, order);
          break;
        case itk::ImageIOBase::FLOAT:
          dv::FlipAlongAxes<DIM, float>(IImage, OImage, order);
          break;
        case itk::ImageIOBase::DOUBLE:
          dv::FlipAlongAxes<DIM, double>(IImage, OImage, order);
          break;
        default:
          std::cerr << "ERROR: Unrecognized pixel type." << std::endl;
          return EXIT_FAILURE;
          break;
      }
      break;
      }
    default:
      std::cerr << "ERROR: Unsupported dimension." << std::endl;
      return EXIT_FAILURE;
      break;
    }

  return EXIT_SUCCESS;
}
