// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkFixedArray.h>

// Custom
#include <dvReadImageIOBase.h>
#include <dvWindowImage.h>

int
main(int argc, char ** argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
  ("help", "Print usage information.")
  ("input-image",  po::value<std::string>()->required(),                       "Filename of the input mesh.")
  ("output-image", po::value<std::string>()->required(),                       "Filename of the output image.")
  ("scale",        po::value<std::vector<double>>()->multitoken()->required(), "Min and max values for window scaling.")
  ("output-range", po::value<std::vector<double>>()->multitoken(),             "Min and max values for output image.")
;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || vm.empty())
  {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
  }

  po::notify(vm);

  const auto IImage = vm["input-image"].as<std::string>();
  const auto OImage = vm["output-image"].as<std::string>();
  const auto scaleVec = vm["scale"].as<std::vector<double>>();

  using TArray = itk::FixedArray<double, 2>;
  TArray scale, o_range;
  for (size_t i = 0; i < 2; ++i)
    {
    scale[i] = scaleVec.at(i);
    }

  const bool o_range_exists = vm.count("output-range");
  if  (o_range_exists)
    {
    const auto outputVec = vm["output-range"].as<std::vector<double>>();
    for (size_t i = 0; i < 2; ++i)
      {
      o_range[i] = outputVec.at(i);
      }
    }

  switch (dv::ReadImageIOBase(IImage)->GetNumberOfDimensions())
    {

    case 2:
      {
      constexpr unsigned int DIM = 2;

      switch (dv::ReadImageIOBase(IImage)->GetComponentType())
        {
        case itk::ImageIOBase::UCHAR:
          dv::WindowImage<DIM, unsigned char>(IImage, OImage, scale, o_range, o_range_exists);
          break;
        case itk::ImageIOBase::CHAR:
          dv::WindowImage<DIM, char>(IImage, OImage, scale, o_range, o_range_exists);
          break;
        case itk::ImageIOBase::USHORT:
          dv::WindowImage<DIM, unsigned short>(IImage, OImage, scale, o_range, o_range_exists);
          break;
        case itk::ImageIOBase::SHORT:
          dv::WindowImage<DIM, short>(IImage, OImage, scale, o_range, o_range_exists);
          break;
        case itk::ImageIOBase::UINT:
          dv::WindowImage<DIM, unsigned int>(IImage, OImage, scale, o_range, o_range_exists);
          break;
        case itk::ImageIOBase::INT:
          dv::WindowImage<DIM, int>(IImage, OImage, scale, o_range, o_range_exists);
          break;
        case itk::ImageIOBase::ULONG:
          dv::WindowImage<DIM, unsigned long>(IImage, OImage, scale, o_range, o_range_exists);
          break;
        case itk::ImageIOBase::LONG:
          dv::WindowImage<DIM, long>(IImage, OImage, scale, o_range, o_range_exists);
          break;
        case itk::ImageIOBase::FLOAT:
          dv::WindowImage<DIM, float>(IImage, OImage, scale, o_range, o_range_exists);
          break;
        case itk::ImageIOBase::DOUBLE:
          dv::WindowImage<DIM, double>(IImage, OImage, scale, o_range, o_range_exists);
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

      switch (dv::ReadImageIOBase(IImage)->GetComponentType())
        {
        case itk::ImageIOBase::UCHAR:
          dv::WindowImage<DIM, unsigned char>(IImage, OImage, scale, o_range, o_range_exists);
          break;
        case itk::ImageIOBase::CHAR:
          dv::WindowImage<DIM, char>(IImage, OImage, scale, o_range, o_range_exists);
          break;
        case itk::ImageIOBase::USHORT:
          dv::WindowImage<DIM, unsigned short>(IImage, OImage, scale, o_range, o_range_exists);
          break;
        case itk::ImageIOBase::SHORT:
          dv::WindowImage<DIM, short>(IImage, OImage, scale, o_range, o_range_exists);
          break;
        case itk::ImageIOBase::UINT:
          dv::WindowImage<DIM, unsigned int>(IImage, OImage, scale, o_range, o_range_exists);
          break;
        case itk::ImageIOBase::INT:
          dv::WindowImage<DIM, int>(IImage, OImage, scale, o_range, o_range_exists);
          break;
        case itk::ImageIOBase::ULONG:
          dv::WindowImage<DIM, unsigned long>(IImage, OImage, scale, o_range, o_range_exists);
          break;
        case itk::ImageIOBase::LONG:
          dv::WindowImage<DIM, long>(IImage, OImage, scale, o_range, o_range_exists);
          break;
        case itk::ImageIOBase::FLOAT:
          dv::WindowImage<DIM, float>(IImage, OImage, scale, o_range, o_range_exists);
          break;
        case itk::ImageIOBase::DOUBLE:
          dv::WindowImage<DIM, double>(IImage, OImage, scale, o_range, o_range_exists);
          break;
        default:
          std::cerr << "ERROR: Unrecognized pixel type." << std::endl;
          return EXIT_FAILURE;
          break;
        }
      break;
      }

    default:
      {
      std::cerr << "ERROR: Image dimension not recognized." << std::endl;
      return EXIT_FAILURE;
      break;
      }
    }

  return EXIT_SUCCESS;

}
