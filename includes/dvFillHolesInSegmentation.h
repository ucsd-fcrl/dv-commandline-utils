#ifndef dv_FillHolesInSegmentation_h
#define dv_FillHolesInSegmentation_h

// ITK
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

// Custom
#include <itkFillHolesInSegmentationImageFilter.h>

namespace dv {

template<unsigned int Dimension, typename TPixel>
void
FillHolesInSegmentation(const std::string& IImage, const std::string& OImage)
{

  using TImage = itk::Image<TPixel, Dimension>;
  using TReader = itk::ImageFileReader<TImage>;
  using TFill = itk::FillHolesInSegmentationImageFilter<TImage>;
  using TWriter = itk::ImageFileWriter<TImage>;

  const auto reader = TReader::New();
  reader->SetFileName(IImage);

  const auto fill = TFill::New();
  fill->SetInput( reader->GetOutput() );

  const auto writer = TWriter::New();
  writer->SetFileName(OImage);
  writer->SetInput( fill->GetOutput() );
  writer->Update();

}

}

#endif
