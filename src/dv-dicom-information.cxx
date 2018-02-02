// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include "itkImageFileReader.h"
#include "itkGDCMImageIO.h"
#include "itkMetaDataObject.h"
#include "gdcmGlobal.h"

// Custom
#include <dvReadImageIOBase.h>

namespace dv
{
template<unsigned int Dimension, typename TPixel>
void
ReadGDCMImage(const std::string& IImage, itk::GDCMImageIO::Pointer dicomIO)
{

  using TImage = itk::Image< TPixel, Dimension >;
  using TReader = itk::ImageFileReader< TImage >;

  const auto reader = TReader::New();

  reader->SetFileName( IImage );
  reader->SetImageIO( dicomIO );
  reader->Update();

}
}

int
main(int argc, char**argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-image", po::value<std::string>()->required(), "Filename of input image.")
    ("dicom-tag", po::value<std::string>()->required(), "DICOM tag in the format: 0028|0030");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const auto IImage = vm["input-image"].as<std::string>();
  const auto Tag = vm["dicom-tag"].as<std::string>();

  const auto dicomIO = itk::GDCMImageIO::New();

  switch (dv::ReadImageIOBase(IImage)->GetNumberOfDimensions())
    {
    case 2:
      dv::ReadGDCMImage<2, signed short>(IImage, dicomIO);
      break;
    case 3:
      dv::ReadGDCMImage<3, signed short>(IImage, dicomIO);
      break;
    case 4:
      dv::ReadGDCMImage<4, signed short>(IImage, dicomIO);
      break;
    default:
      std::cerr << "Image dimension not supported." << std::endl;
      return EXIT_FAILURE;
    }

  const auto dictionary = dicomIO->GetMetaDataDictionary();

  using TMetaDataString = itk::MetaDataObject< std::string >;

  const auto tagItr = dictionary.Find( Tag );

  if (tagItr == dictionary.End())
    {
    std::cerr << "The tag could not be found." << std::endl;
    return EXIT_FAILURE;
    }

  const auto entryvalue =
    dynamic_cast<const TMetaDataString *>( tagItr->second.GetPointer() );

  if (!entryvalue)
    {
    std::cerr << "The tag value cound not be converted to a string." << std::endl;
    return EXIT_FAILURE;
    }

  const std::string tagvalue = entryvalue->GetMetaDataObjectValue();
  std::cout << tagvalue << std::endl;

  return EXIT_SUCCESS;

}
