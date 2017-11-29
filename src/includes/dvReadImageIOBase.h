#ifndef dv_ReadImageIOBase_h
#define dv_ReadImageIOBase_h

#include <string>
#include <itkImageFileReader.h>
#include <itkImageIOBase.h>

namespace dv
{

itk::ImageIOBase::Pointer
ReadImageIOBase(const std::string &fileName)
{

  itk::ImageIOBase::Pointer imageIO =
    itk::ImageIOFactory::CreateImageIO(
      fileName.c_str(),
      itk::ImageIOFactory::ReadMode );

  if (nullptr == imageIO)
    {
    std::cerr << "ERROR: Image IO was not created." << std::endl;
    return nullptr;
    }

  imageIO->SetFileName( fileName );
  imageIO->ReadImageInformation();

  return imageIO;

}

}

#endif
