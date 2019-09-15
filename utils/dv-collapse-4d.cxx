// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

#include <dvCollapse4D.h>
#include <dvReadImageIOBase.h>

int
main(int argc, char **argv)
{

  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-image",           po::value<std::string>()->required(), "Filename of the input image.")
    ("dimension-to-collapse", po::value<unsigned int>()->required(), "Dimension to collapse.")
    ("output-image",          po::value<std::string>()->required(), "Filename of the output image.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc)
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const auto IImage        = vm["input-image"].as<std::string>();
  const auto DimToCollapse = vm["dimension-to-collapse"].as<unsigned int>();
  const auto OImage        = vm["output-image"].as<std::string>();

  if (DimToCollapse > 3)
    {
    std::cerr << "Invalid dimension supplied: " << DimToCollapse << std::endl;
    return EXIT_FAILURE;
    }

  switch (dv::ReadImageIOBase(IImage)->GetComponentType())
    {
    case itk::ImageIOBase::UCHAR:
      dv::Collapse4D<3, unsigned char>(IImage, DimToCollapse, OImage);
      break;
    case itk::ImageIOBase::CHAR:
      dv::Collapse4D<3, char>(IImage, DimToCollapse, OImage);
      break;
    case itk::ImageIOBase::USHORT:
      dv::Collapse4D<3, unsigned short>(IImage, DimToCollapse, OImage);
      break;
    case itk::ImageIOBase::SHORT:
      dv::Collapse4D<3, short>(IImage, DimToCollapse, OImage);
      break;
    case itk::ImageIOBase::UINT:
      dv::Collapse4D<3, unsigned int>(IImage, DimToCollapse, OImage);
      break;
    case itk::ImageIOBase::INT:
      dv::Collapse4D<3, int>(IImage, DimToCollapse, OImage);
      break;
    case itk::ImageIOBase::ULONG:
      dv::Collapse4D<3, unsigned long>(IImage, DimToCollapse, OImage);
      break;
    case itk::ImageIOBase::LONG:
      dv::Collapse4D<3, long>(IImage, DimToCollapse, OImage);
      break;
    case itk::ImageIOBase::FLOAT:
      dv::Collapse4D<3, float>(IImage, DimToCollapse, OImage);
      break;
    case itk::ImageIOBase::DOUBLE:
      dv::Collapse4D<3, double>(IImage, DimToCollapse, OImage);
      break;
    default:
      std::cerr << "ERROR: Unrecognized pixel type." << std::endl;
      return EXIT_FAILURE;
      break;
    }

  return EXIT_SUCCESS;

}

