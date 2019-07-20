// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Custom
#include <dvReadImageIOBase.h>
#include <dvExtractOrthogonalSlice.h>

int
main( int argc, char* argv[] )
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help",         "Print usage information.")
    ("input-image",  po::value<std::string>()->required(),  "Filename of the input image.")
    ("output-image", po::value<std::string>()->required(),  "Filename of the output image.")
    ("dimension",    po::value<unsigned int>()->required(), "Dimension.")
    ("slice",        po::value<unsigned int>()->required(), "Index.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || vm.empty())
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const std::string IImage(vm["input-image"].as<std::string>());
  const std::string OImage(vm["output-image"].as<std::string>());
  const unsigned int DimensionIndex(vm["dimension"].as<unsigned int>());
  const unsigned int SliceIndex(vm["slice"].as<unsigned int>());

  switch (dv::ReadImageIOBase(IImage)->GetComponentType())
    {
    case itk::ImageIOBase::UCHAR:
      dv::ExtractOrthogonalSlice<3, unsigned char>(IImage, OImage, DimensionIndex, SliceIndex);
      break;
    case itk::ImageIOBase::CHAR:
      dv::ExtractOrthogonalSlice<3, char>(IImage, OImage, DimensionIndex, SliceIndex);
      break;
    case itk::ImageIOBase::USHORT:
      dv::ExtractOrthogonalSlice<3, unsigned short>(IImage, OImage, DimensionIndex, SliceIndex);
      break;
    case itk::ImageIOBase::SHORT:
      dv::ExtractOrthogonalSlice<3, short>(IImage, OImage, DimensionIndex, SliceIndex);
      break;
    case itk::ImageIOBase::UINT:
      dv::ExtractOrthogonalSlice<3, unsigned int>(IImage, OImage, DimensionIndex, SliceIndex);
      break;
    case itk::ImageIOBase::INT:
      dv::ExtractOrthogonalSlice<3, int>(IImage, OImage, DimensionIndex, SliceIndex);
      break;
    case itk::ImageIOBase::ULONG:
      dv::ExtractOrthogonalSlice<3, unsigned long>(IImage, OImage, DimensionIndex, SliceIndex);
      break;
    case itk::ImageIOBase::LONG:
      dv::ExtractOrthogonalSlice<3, long>(IImage, OImage, DimensionIndex, SliceIndex);
      break;
    case itk::ImageIOBase::FLOAT:
      dv::ExtractOrthogonalSlice<3, float>(IImage, OImage, DimensionIndex, SliceIndex);
      break;
    case itk::ImageIOBase::DOUBLE:
      dv::ExtractOrthogonalSlice<3, double>(IImage, OImage, DimensionIndex, SliceIndex);
      break;
    default:
      std::cerr << "ERROR: Unrecognized pixel type." << std::endl;
      return EXIT_FAILURE;
      break;
    }

  return EXIT_SUCCESS;

}

