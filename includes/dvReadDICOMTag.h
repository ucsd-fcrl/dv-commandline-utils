#ifndef dv_ReadDICOMTag_h
#define dv_ReadDICOMTag_h

// ITK
#include "itkImageFileReader.h"
#include "itkGDCMImageIO.h"
#include "itkMetaDataObject.h"
#include "gdcmGlobal.h"

// Custom
#include <dvReadImageIOBase.h>

using TReturn = std::tuple<bool, std::string, std::string>;

namespace dv
{
template<unsigned int Dimension, typename TPixel>
void
ReadGDCMImage(const std::string& IImage, itk::GDCMImageIO::Pointer dicomIO);

TReturn
ReadDICOMTag(const std::string& IImage, const std::string& Tag);
}

#endif
