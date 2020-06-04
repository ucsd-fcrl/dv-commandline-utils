#ifndef dv_EnforceBoundaryBetweenLabels_h
#define dv_EnforceBoundaryBetweenLabels_h

// ITK
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

// Custom
#include "itkEnforceBoundaryBetweenLabelsImageFilter.h"

namespace dv {

template<unsigned int Dimension, typename TPixel>
void
EnforceBoundaryBetweenLabels(const std::string& IImage,
                             const std::string& OImage,
                             const std::set<TPixel>& labels_1,
                             const std::set<TPixel>& labels_2,
                             const short radius_1,
                             const short radius_2,
                             const short replacement)
{

  using TImage = itk::Image<TPixel, Dimension>;
  using TReader = itk::ImageFileReader<TImage>;
  using TFilter = itk::EnforceBoundaryBetweenLabelsImageFilter<TImage>;
  using TWriter = itk::ImageFileWriter<TImage>;

  //
  // Reader
  //

  const auto reader = TReader::New();
  reader->SetFileName(IImage);

  const auto filter = TFilter::New();
  filter->SetInput(reader->GetOutput());
  filter->SetLabels1(labels_1);
  filter->SetLabels2(labels_2);
  filter->SetRadius1(radius_1);
  filter->SetRadius2(radius_2);
  filter->SetReplacement(replacement);

  const auto writer = TWriter::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(OImage);
  writer->Update();
}

}

#endif
