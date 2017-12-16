#ifndef dv_FillHolesInBinaryImage_h
#define dv_FillHolesInBinaryImage_h

#include <itkImageFileReader.h>
#include <itkBinaryFillholeImageFilter.h>
#include <itkImageFileWriter.h>

namespace dv
{

template<unsigned int Dimension, typename TPixel>
void
FillHolesInBinaryImage(const std::string& IImage, const std::string& OImage)
{
  typedef itk::Image< TPixel, Dimension > TImage;
  typedef itk::ImageFileReader< TImage >  TReader;
  typedef itk::BinaryFillholeImageFilter< TImage > TFilter;
  typedef itk::ImageFileWriter< TImage > TWriter;

  const auto reader = TReader::New();
  reader->SetFileName( IImage );

  const auto filter = TFilter::New();
  filter->SetInput( reader->GetOutput() );
  filter->SetForegroundValue( static_cast<TPixel>(1) );

  const auto writer = TWriter::New();
  writer->SetFileName( OImage );
  writer->SetInput( filter->GetOutput() );
  writer->Update();
}

}

#endif
