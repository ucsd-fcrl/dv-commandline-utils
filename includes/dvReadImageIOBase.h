#ifndef dv_ReadImageIOBase_h
#define dv_ReadImageIOBase_h

#include <itkImageFileReader.h>
#include <itkImageIOBase.h>
#include <string>

namespace dv {

itk::ImageIOBase::Pointer
ReadImageIOBase(const std::string& fileName);

}

#endif
