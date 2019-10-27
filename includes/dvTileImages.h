#ifndef dv_TileImages_h
#define dv_TileImages_h

// ITK
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkTileImageFilter.h"

namespace dv {

template<unsigned int Dimension, typename TPixel>
void
TileImages(const std::vector<std::string>& InputImages,
           const std::string& OutputImage,
           const unsigned int& NumRows,
           const unsigned int& NumCols,
           const TPixel& Background)
{

  using InputImageType = itk::Image<TPixel, Dimension>;
  using OutputImageType = itk::Image<TPixel, Dimension>;
  using ImageReaderType = itk::ImageFileReader<InputImageType>;
  using TilerType = itk::TileImageFilter<InputImageType, OutputImageType>;
  using WriterType = itk::ImageFileWriter<OutputImageType>;

  const auto tiler = TilerType::New();
  itk::FixedArray<unsigned int, Dimension> layout;
  layout[0] = NumCols;
  layout[1] = NumRows;
  layout[2] = 0;

  tiler->SetLayout(layout);
  tiler->SetDefaultPixelValue(Background);

  for (size_t i = 0; i < InputImages.size(); ++i) {
    const auto reader = ImageReaderType::New();
    reader->SetFileName(InputImages[i]);
    reader->Update();
    tiler->SetInput(i, reader->GetOutput());
  }

  tiler->Update();

  const auto writer = WriterType::New();
  writer->SetInput(tiler->GetOutput());
  writer->SetFileName(OutputImage);
  writer->Update();
}

}

#endif
