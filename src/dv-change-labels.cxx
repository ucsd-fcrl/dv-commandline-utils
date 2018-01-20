// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Custom
#include <dvReadImageIOBase.h>
#include <dvChangeLabels.h>

const unsigned int Dimension = 3;

int
main( int argc, char** argv )
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-image",  po::value<std::string>()->required(),          "Filename of the input image.")
    ("output-image", po::value<std::string>()->required(),          "Filename of the output image.")
    ("label-map",    po::value<std::vector<short>>()->multitoken(), "Label map: 1 2 2 1 => {1 : 2, 2 : 1}.")
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
  const std::string OImage(vm["output-image"].as<std::string>());
  const std::vector<short> labelVector = vm["label-map"].as<std::vector<short>>();

  assert(labelVector.size() % 2 == 0);

  //
  // make Set from labels given
  //


  switch (dv::ReadImageIOBase(IImage)->GetComponentType())
    {
    case itk::ImageIOBase::UCHAR:
      {
      std::map<unsigned char, unsigned char> labelMap;
      for (size_t i = 0; i < labelVector.size(); i += 2)
        labelMap[labelVector[i]] = labelVector[i + 1];
      dv::ChangeLabels<3, unsigned char>(IImage, labelMap, OImage);
      break;
      }
    case itk::ImageIOBase::CHAR:
      {
      std::map<char, char> labelMap;
      for (size_t i = 0; i < labelVector.size(); i += 2)
        labelMap[labelVector[i]] = labelVector[i + 1];
      dv::ChangeLabels<3, char>(IImage, labelMap, OImage);
      break;
      }
    case itk::ImageIOBase::USHORT:
      {
      std::map<unsigned short, unsigned short> labelMap;
      for (size_t i = 0; i < labelVector.size(); i += 2)
        labelMap[labelVector[i]] = labelVector[i + 1];
      dv::ChangeLabels<3, unsigned short>(IImage, labelMap, OImage);
      break;
      }
    case itk::ImageIOBase::SHORT:
      {
      std::map<short, short> labelMap;
      for (size_t i = 0; i < labelVector.size(); i += 2)
        labelMap[labelVector[i]] = labelVector[i + 1];
      dv::ChangeLabels<3, short>(IImage, labelMap, OImage);
      break;
      }
    case itk::ImageIOBase::UINT:
      {
      std::map<unsigned int, unsigned int> labelMap;
      for (size_t i = 0; i < labelVector.size(); i += 2)
        labelMap[labelVector[i]] = labelVector[i + 1];
      dv::ChangeLabels<3, unsigned int>(IImage, labelMap, OImage);
      break;
      }
    case itk::ImageIOBase::INT:
      {
      std::map<int, int> labelMap;
      for (size_t i = 0; i < labelVector.size(); i += 2)
        labelMap[labelVector[i]] = labelVector[i + 1];
      dv::ChangeLabels<3, int>(IImage, labelMap, OImage);
      break;
      }
    case itk::ImageIOBase::ULONG:
      {
      std::map<unsigned long, unsigned long> labelMap;
      for (size_t i = 0; i < labelVector.size(); i += 2)
        labelMap[labelVector[i]] = labelVector[i + 1];
      dv::ChangeLabels<3, unsigned long>(IImage, labelMap, OImage);
      break;
      }
    case itk::ImageIOBase::LONG:
      {
      std::map<long, long> labelMap;
      for (size_t i = 0; i < labelVector.size(); i += 2)
        labelMap[labelVector[i]] = labelVector[i + 1];
      dv::ChangeLabels<3, long>(IImage, labelMap, OImage);
      break;
      }
    default:
      std::cerr << "ERROR: Unrecognized pixel type." << std::endl;
      return EXIT_FAILURE;
      break;
    }





  return EXIT_SUCCESS;

}
