#include <dvReadDICOMTag.h>

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


std::tuple<bool, std::string, std::string>
ReadDICOMTag(const std::string& IImage, const std::string& Tag)
{
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
      return TReturn{false, "Image dimension not supported.", ""};
    }

  const auto dictionary = dicomIO->GetMetaDataDictionary();

  using TMetaDataString = itk::MetaDataObject< std::string >;

  const auto tagItr = dictionary.Find( Tag );

  if (tagItr == dictionary.End())
    {
    return TReturn{false, "The tag could not be found.", ""};
    }

  const auto entryvalue =
    dynamic_cast<const TMetaDataString *>( tagItr->second.GetPointer() );

  if (!entryvalue)
    {
    return TReturn{false, "The tag could not be converted.", ""};
    }

  const std::string tagvalue = entryvalue->GetMetaDataObjectValue();
  return TReturn{true, "", tagvalue};

}

}
