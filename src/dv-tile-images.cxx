// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Custom
#include <dvReadImageIOBase.h>
#include <dvTileImages.h>

int
main(int argc, char **argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help",           "Print usage information.")
    ("input-images", po::value<std::vector<std::string>>()->multitoken()->required(), "List of input images.")
    ("output-image", po::value<std::string>()->required(), "Output image.")
    ("rows",         po::value<unsigned int>(), "Number of rows in tiled output.")
    ("columns",      po::value<unsigned int>(), "Number of columns in tiled output.")
    ("background",   po::value<double>()->default_value(0.0), "Default value (for unused tiles).")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || vm.empty())
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const auto InputImages = vm["input-images"].as<std::vector<std::string>>();
  const auto OutputImage = vm["output-image"].as<std::string>();
  const auto NumRows = vm["rows"].as<unsigned int>();
  const auto NumCols = vm["columns"].as<unsigned int>();
  const auto Background = vm["background"].as<double>();

  itkAssertOrThrowMacro(InputImages.size() <= (NumRows * NumCols), "Too many images have been supplied.");

  switch (dv::ReadImageIOBase(InputImages[0])->GetComponentType())
    {
    case itk::ImageIOBase::UCHAR:
      switch (dv::ReadImageIOBase(InputImages[0])->GetPixelType())
      {
        case itk::ImageIOBase::RGB:   /* If RGB... */
        {
          typedef itk::RGBPixel< unsigned char > PixelType;
          dv::TileImages<2, PixelType>(InputImages, OutputImage, NumRows, NumCols, static_cast<unsigned char>(Background));
          break;
        }

        case itk::ImageIOBase::RGBA: /* If RGBA... */
        {
          typedef itk::RGBAPixel< unsigned char > PixelType;
          dv::TileImages<2, PixelType>(InputImages, OutputImage, NumRows, NumCols, static_cast<unsigned char>(Background));
          break;
        }
        case itk::ImageIOBase::SCALAR: /* regular */
          dv::TileImages<2, unsigned char>(InputImages, OutputImage, NumRows, NumCols, static_cast<unsigned char>(Background));
          break;
        default:
          std::cerr << "ERROR: Unrecognized pixel type." << std::endl;
          return EXIT_FAILURE;
          break;
      }
      break;
    case itk::ImageIOBase::CHAR:
      dv::TileImages<2, char>(InputImages, OutputImage, NumRows, NumCols, static_cast<char>(Background));
      break;
    case itk::ImageIOBase::USHORT:
      switch (dv::ReadImageIOBase(InputImages[0])->GetPixelType())
      {
        case itk::ImageIOBase::RGB:   /* If RGB... */
        {
          typedef itk::RGBPixel< unsigned short > PixelType;
          dv::TileImages<2, PixelType>(InputImages, OutputImage, NumRows, NumCols, static_cast<unsigned char>(Background));
          break;
        }
        case itk::ImageIOBase::RGBA: /* If RGBA... */
        {
          typedef itk::RGBAPixel< unsigned short > PixelType;
          dv::TileImages<2, PixelType>(InputImages, OutputImage, NumRows, NumCols, static_cast<unsigned char>(Background));
          break;
        }
        case itk::ImageIOBase::SCALAR: /* regular */
          dv::TileImages<2, unsigned short>(InputImages, OutputImage, NumRows, NumCols, static_cast<unsigned short>(Background));
          break;
        default:
          std::cerr << "ERROR: Unrecognized pixel type." << std::endl;
          return EXIT_FAILURE;
          break;
      }
      break;
    case itk::ImageIOBase::SHORT:
      dv::TileImages<2, short>(InputImages, OutputImage, NumRows, NumCols, static_cast<short>(Background));
      break;
    case itk::ImageIOBase::UINT:
      dv::TileImages<2, unsigned int>(InputImages, OutputImage, NumRows, NumCols, static_cast<unsigned int>(Background));
      break;
    case itk::ImageIOBase::INT:
      dv::TileImages<2, int>(InputImages, OutputImage, NumRows, NumCols, static_cast<int>(Background));
      break;
    case itk::ImageIOBase::ULONG:
      dv::TileImages<2, unsigned long>(InputImages, OutputImage, NumRows, NumCols, static_cast<unsigned long>(Background));
      break;
    case itk::ImageIOBase::LONG:
      dv::TileImages<2, long>(InputImages, OutputImage, NumRows, NumCols, static_cast<long>(Background));
      break;
    case itk::ImageIOBase::FLOAT:
      dv::TileImages<2, float>(InputImages, OutputImage, NumRows, NumCols, static_cast<float>(Background));
      break;
    case itk::ImageIOBase::DOUBLE:
      dv::TileImages<2, double>(InputImages, OutputImage, NumRows, NumCols, static_cast<double>(Background));
      break;
    default:
      std::cerr << "ERROR: Unrecognized pixel type." << std::endl;
      return EXIT_FAILURE;
      break;
    }

  return EXIT_SUCCESS;
}
