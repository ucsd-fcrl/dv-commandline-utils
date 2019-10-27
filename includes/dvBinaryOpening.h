#ifndef dv_BinaryOpening_h
#define dv_BinaryOpening_h

#include <itkBinaryBallStructuringElement.h>
#include <itkBinaryMorphologicalOpeningImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

namespace dv {

template<unsigned int Dimension, typename TPixel>
void
BinaryOpening(const std::string& IImage,
              const std::string& OImage,
              const unsigned int radius)
{
  typedef itk::Image<TPixel, Dimension> TImage;
  typedef itk::ImageFileReader<TImage> TReader;
  typedef itk::BinaryBallStructuringElement<TPixel, Dimension> TElement;
  typedef itk::BinaryMorphologicalOpeningImageFilter<TImage, TImage, TElement>
    TOpen;
  typedef itk::ImageFileWriter<TImage> TWriter;

  const auto reader = TReader::New();
  reader->SetFileName(IImage);

  TElement structuringElement;
  structuringElement.SetRadius(radius);
  structuringElement.CreateStructuringElement();

  const auto openingFilter = TOpen::New();
  openingFilter->SetInput(reader->GetOutput());
  openingFilter->SetKernel(structuringElement);
  openingFilter->SetForegroundValue(1);
  openingFilter->SetBackgroundValue(0);

  const auto writer = TWriter::New();
  writer->SetFileName(OImage);
  writer->SetInput(openingFilter->GetOutput());
  writer->Update();
}

}

#endif
