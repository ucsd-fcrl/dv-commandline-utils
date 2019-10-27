#ifndef dv_ChangeInformation3D_h
#define dv_ChangeInformation3D_h

// ITK
#include <itkChangeInformationImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

namespace dv {

template<unsigned int Dimension, typename TPixel>
void
ChangeInformation3D(const std::string& IImage,
                    const std::string& RImage,
                    const std::string& OImage)
{
  using TImage = itk::Image<TPixel, Dimension>;
  using TReader = itk::ImageFileReader<TImage>;
  using TInfo = itk::ChangeInformationImageFilter<TImage>;
  using TWriter = itk::ImageFileWriter<TImage>;

  const auto referenceReader = TReader::New();
  referenceReader->SetFileName(RImage);
  referenceReader->Update();

  const auto reader = TReader::New();
  reader->SetFileName(IImage);

  const auto info = TInfo::New();
  info->SetInput(reader->GetOutput());
  info->SetReferenceImage(referenceReader->GetOutput());
  info->UseReferenceImageOn();
  info->ChangeAll();

  const auto writer = TWriter::New();
  writer->SetInput(info->GetOutput());
  writer->SetFileName(OImage);
  writer->Update();
}

}

#endif
