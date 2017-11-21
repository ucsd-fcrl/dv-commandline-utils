// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include "itkImageFileReader.h"
#include "itkGDCMImageIO.h"
#include "itkMetaDataObject.h"
#include "gdcmGlobal.h"

template<unsigned int Dimension, typename TPixel>
void
read(po::variables_map vm, itk::GDCMImageIO::Pointer dicomIO)
{

  using TImage = itk::Image< TPixel, Dimension >;
  using TReader = itk::ImageFileReader< TImage >;

  const auto reader = TReader::New();

  reader->SetFileName( vm["input-image"].as<std::string>() );
  reader->SetImageIO( dicomIO );
  reader->Update();

}

int
main(int argc, char**argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-image", po::value<std::string>()->required(), "Filename of input image.")
    ("dicom-tag", po::value<std::string>()->required(), "DICOM tag in the format: 0028|0030")
    ("image-dimension", po::value<unsigned int>()->required(), "The dimension of the image (2, 3, or 4 supported)");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const auto dicomIO = itk::GDCMImageIO::New();

  const unsigned int Dimension = vm["image-dimension"].as<unsigned int>();

  switch (Dimension)
    {
    case 2:
      read<2, signed short>(vm, dicomIO);
      break;
    case 3:
      read<3, signed short>(vm, dicomIO);
      break;
    case 4:
      read<4, signed short>(vm, dicomIO);
      break;
    default:
      std::cerr << "Image dimension not supported." << std::endl;
      return EXIT_FAILURE;
    }

  typedef itk::MetaDataDictionary   DictionaryType;
  const  DictionaryType & dictionary = dicomIO->GetMetaDataDictionary();

  typedef itk::MetaDataObject< std::string > MetaDataStringType;

  DictionaryType::ConstIterator tagItr = dictionary.Find( vm["dicom-tag"].as<std::string>() );

  if (tagItr == dictionary.End())
    {
    std::cerr << "The tag could not be found." << std::endl;
    return EXIT_FAILURE;
    }

  MetaDataStringType::ConstPointer entryvalue =
    dynamic_cast<const MetaDataStringType *>( tagItr->second.GetPointer() );

  if (!entryvalue)
    {
    std::cerr << "The tag value cound not be converted to a string." << std::endl;
    return EXIT_FAILURE;
    }

  const std::string tagvalue = entryvalue->GetMetaDataObjectValue();
  std::cout << tagvalue.c_str() << std::endl;

  return EXIT_SUCCESS;

}
