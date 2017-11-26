// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkImageFileReader.h>
#include <itkImageIOBase.h>
#include "includes/dvImageInformationFunctors.h"
#include "includes/dvReadImageIOBase.h"

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

  const auto imageIO = dv::ReadImageIOBase(inputFileName);

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

