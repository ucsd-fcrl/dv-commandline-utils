#ifndef dv_ImageInformationFunctors_h
#define dv_ImageInformationFunctors_h

namespace dv
{

struct GetInfoFunction
{
  const std::string key;
  virtual std::string
  GetInfo(itk::ImageIOBase::Pointer imageIO) = 0;
  protected: GetInfoFunction(std::string _key) : key(_key){};
};

struct GetSummaryTypeFunction
: public GetInfoFunction
{
  GetSummaryTypeFunction():GetInfoFunction("--summary"){};
  std::string
  GetInfo(itk::ImageIOBase::Pointer imageIO) final
    {
    std::stringstream ss;
    imageIO->Print(ss);
    return ss.str();
    }
};

struct GetPixelTypeFunction
: public GetInfoFunction
{
  GetPixelTypeFunction():GetInfoFunction("--pixel-type"){};
  std::string
  GetInfo(itk::ImageIOBase::Pointer imageIO) final
    {  
    const auto pixelType = imageIO->GetPixelType();
    return itk::ImageIOBase::GetPixelTypeAsString( pixelType );
    }
};

struct GetComponentTypeFunction
: public GetInfoFunction
{
  GetComponentTypeFunction():GetInfoFunction("--component-type"){};
  std::string
  GetInfo(itk::ImageIOBase::Pointer imageIO) final
    {  
    const auto componentType = imageIO->GetComponentType();
    return imageIO->GetComponentTypeAsString( componentType );
    }
};

struct GetFileTypeFunction
: public GetInfoFunction
{
  GetFileTypeFunction():GetInfoFunction("--file-type"){};
  std::string
  GetInfo(itk::ImageIOBase::Pointer imageIO) final
    {  
    const auto fileType = imageIO->GetFileType();
    return imageIO->GetFileTypeAsString( fileType );
    }
};


struct GetDimensionFunction
: public GetInfoFunction
{
  GetDimensionFunction():GetInfoFunction("--dimension"){};
  std::string
  GetInfo(itk::ImageIOBase::Pointer imageIO) final
    {  
    return std::to_string(imageIO->GetNumberOfDimensions());
    }
};

struct GetDimensionsFunction
: public GetInfoFunction
{
  GetDimensionsFunction():GetInfoFunction("--dimensions"){};
  std::string
  GetInfo(itk::ImageIOBase::Pointer imageIO) final
    {  
    const auto N = imageIO->GetNumberOfDimensions();
    std::string dim;
    for (std::size_t i = 0; i < N; ++i) dim += std::to_string(imageIO->GetDimensions(i)) + ' ';
    return dim;
    }
};

struct GetOriginFunction
: public GetInfoFunction
{
  GetOriginFunction():GetInfoFunction("--origin"){};
  std::string
  GetInfo(itk::ImageIOBase::Pointer imageIO) final
    {
    const auto N = imageIO->GetNumberOfDimensions();
    std::string origin;
    for (std::size_t i = 0; i < N; ++i) origin += std::to_string(imageIO->GetOrigin(i)) + ' ';
    return origin;
    }
};

struct GetSpacingFunction
: public GetInfoFunction
{
  GetSpacingFunction():GetInfoFunction("--spacing"){};
  std::string
  GetInfo(itk::ImageIOBase::Pointer imageIO) final
    {
    const auto N = imageIO->GetNumberOfDimensions();
    std::string spacing;
    for (std::size_t i = 0; i < N; ++i) spacing += std::to_string(imageIO->GetSpacing(i)) + ' ';
    return spacing;
    }
};

struct GetDirectionFunction
: public GetInfoFunction
{
  GetDirectionFunction():GetInfoFunction("--direction"){};
  std::string
  GetInfo(itk::ImageIOBase::Pointer imageIO) final
    {
    const auto N = imageIO->GetNumberOfDimensions();
    std::string direction;
    for (std::size_t i = 0; i < N; ++i)
      {
      const auto vec = imageIO->GetDirection(i);
      for (const auto &v : vec)
        {
        direction += std::to_string(v) + ' ';
        }
      }
    return direction;
    }
};

struct GetSizeInPixelsFunction
: public GetInfoFunction
{
  GetSizeInPixelsFunction():GetInfoFunction("--size-in-pixels"){};
  std::string
  GetInfo(itk::ImageIOBase::Pointer imageIO) final
    {
    return std::to_string(imageIO->GetImageSizeInPixels());
    }
};

struct GetSizeInComponentsFunction
: public GetInfoFunction
{
  GetSizeInComponentsFunction():GetInfoFunction("--size-in-components"){};
  std::string
  GetInfo(itk::ImageIOBase::Pointer imageIO) final
    {
    return std::to_string(imageIO->GetImageSizeInComponents());
    }
};

struct GetPixelStrideFunction
: public GetInfoFunction
{
  GetPixelStrideFunction():GetInfoFunction("--pixel-stride"){};
  std::string
  GetInfo(itk::ImageIOBase::Pointer imageIO) final
    {
    return std::to_string(imageIO->GetPixelStride());
    }
};

struct GetNumberOfComponentsFunction
: public GetInfoFunction
{
  GetNumberOfComponentsFunction():GetInfoFunction("--number-of-components"){};
  std::string
  GetInfo(itk::ImageIOBase::Pointer imageIO) final
    {
    return std::to_string(imageIO->GetNumberOfComponents());
    }
};

struct GetByteOrderFunction
: public GetInfoFunction
{
  GetByteOrderFunction():GetInfoFunction("--byte-order"){};
  std::string
  GetInfo(itk::ImageIOBase::Pointer imageIO) final
    {
    return std::to_string(imageIO->GetByteOrder());
    }
};

}

#endif
