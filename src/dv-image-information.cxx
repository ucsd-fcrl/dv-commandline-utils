#include "itkImageFileReader.h"
#include "itkImageIOBase.h"

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

int
main( int argc, char** argv )
{

  std::vector<dv::GetInfoFunction*> FunctionMap;

  dv::GetSummaryTypeFunction        summary;
  dv::GetPixelTypeFunction          pixel_type;
  dv::GetComponentTypeFunction      component_type;
  dv::GetFileTypeFunction           file_type;
  dv::GetDimensionFunction          dimension;
  dv::GetDimensionsFunction         dimensions;
  dv::GetOriginFunction             origin;
  dv::GetSpacingFunction            spacing;
  dv::GetDirectionFunction          direction;
  dv::GetSizeInPixelsFunction       size_in_pixels;
  dv::GetSizeInComponentsFunction   size_in_components;
  dv::GetPixelStrideFunction        pixel_stride;
  dv::GetNumberOfComponentsFunction number_of_components;
  dv::GetByteOrderFunction          byte_order;

  FunctionMap.emplace_back(&summary);
  FunctionMap.emplace_back(&pixel_type);
  FunctionMap.emplace_back(&component_type);
  FunctionMap.emplace_back(&file_type);
  FunctionMap.emplace_back(&dimension);
  FunctionMap.emplace_back(&dimensions);
  FunctionMap.emplace_back(&origin);
  FunctionMap.emplace_back(&spacing);
  FunctionMap.emplace_back(&direction);
  FunctionMap.emplace_back(&size_in_pixels);
  FunctionMap.emplace_back(&size_in_components);
  FunctionMap.emplace_back(&pixel_stride);
  FunctionMap.emplace_back(&number_of_components);
  FunctionMap.emplace_back(&byte_order);

  if ( argc != 3 )
    {
    std::cerr << "Usage:\n"
              << argv[0]
              << " <InputFileName>"
              << " <Option>\n";
    std::cerr << "Available options:\n";
    for (const auto &f : FunctionMap)
      std::cerr << f->key << '\n';
    std::cerr << std::flush;
    return EXIT_FAILURE;
    }

  const std::string inputFileName(argv[1]);
  const std::string option(argv[2]);

  itk::ImageIOBase::Pointer imageIO =
    itk::ImageIOFactory::CreateImageIO(
      inputFileName.c_str(),
      itk::ImageIOFactory::ReadMode );

  if (nullptr == imageIO)
    {
    std::cout << "ERROR: Image IO was not created." << std::endl;
    return EXIT_FAILURE;
    }

  imageIO->SetFileName( inputFileName );
  imageIO->ReadImageInformation();

  bool found = false;
  for (const auto &f : FunctionMap)
    {
    if (option != f->key) continue;
    std::cout << f->GetInfo(imageIO) << std::endl;
    found = true;
    }
  if (!found)
    {
    std::cerr << "ERROR: Option not recognized." << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}


