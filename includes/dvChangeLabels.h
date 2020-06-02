#ifndef dv_ChangeLabels_h
#define dv_ChangeLabels_h

// ITK
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkChangeLabelImageFilter.h>

// STD
#include <map>

namespace dv {

template<unsigned int Dimension, typename TPixel>
void
ChangeLabels(const std::string& IImage,
             const std::map<TPixel, TPixel>& LabelMap,
             const std::string& OImage)
{

  using TImage = itk::Image<TPixel, Dimension>;
  using TReader = itk::ImageFileReader<TImage>;
  using TFilter = itk::ChangeLabelImageFilter<TImage,TImage>;
  using TWriter = itk::ImageFileWriter<TImage>;

  //
  // Reader
  //

  const auto reader = TReader::New();
  reader->SetFileName(IImage);

  //
  // Map Labels
  //

  const auto filter = TFilter::New();
  filter->SetChangeMap(LabelMap);
  filter->SetInput(reader->GetOutput());

  //
  // Writer
  //

  const auto writer = TWriter::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(OImage);
  writer->Update();
}

}

#endif
