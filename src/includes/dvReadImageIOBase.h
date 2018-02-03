#ifndef dv_ReadImageIOBase_h
#define dv_ReadImageIOBase_h

#include <string>
#include <itkImageFileReader.h>
#include <itkImageIOBase.h>

namespace dv
{

itk::ImageIOBase::Pointer
ReadImageIOBase(const std::string &fileName);

}

#endif
