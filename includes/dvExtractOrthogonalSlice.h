#ifndef dv_ExtractOrthogonalSlice_h
#define dv_ExtractOrthogonalSlice_h

#include <itkExtractImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

namespace dv {

template<unsigned int Dimension, typename PixelType>
void
ExtractOrthogonalSlice(const std::string& IImage,
                       const std::string& OImage,
                       const unsigned int DimensionIndex,
                       const unsigned int SliceIndex)
{

  using TImage = itk::Image<PixelType, Dimension>;
  using TSlice = itk::Image<PixelType, Dimension - 1>;
  using TReader = itk::ImageFileReader<TImage>;
  using TWriter = itk::ImageFileWriter<TSlice>;
  using TExtract = itk::ExtractImageFilter<TImage, TSlice>;

  const auto reader = TReader::New();
  reader->SetFileName(IImage);
  reader->Update();

  typename TImage::RegionType region =
    reader->GetOutput()->GetLargestPossibleRegion();
  region.GetModifiableSize()[DimensionIndex] = 0;
  region.GetModifiableIndex()[DimensionIndex] = SliceIndex;

  const auto extract = TExtract::New();
  extract->SetInput(reader->GetOutput());
  extract->SetExtractionRegion(region);
  extract->SetDirectionCollapseToStrategy(
    TExtract::DIRECTIONCOLLAPSETOSUBMATRIX);

  const auto writer = TWriter::New();
  writer->SetInput(extract->GetOutput());
  writer->SetFileName(OImage);
  writer->Update();
}

}

#endif
