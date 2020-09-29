#ifndef dv_ExtractConnectedComponents_h
#define dv_ExtractConnectedComponents_h

#include <itkImageFileReader.h>
#include <itkExtractConnectedComponentsImageFilter.h>
#include <itkImageFileWriter.h>

namespace dv {

template<unsigned int Dimension, typename TPixel>
void
ExtractConnectedComponents(const std::string& IImage,
                           const std::string& OImage,
                           const unsigned int N)
{

  using TImage = itk::Image<TPixel, Dimension>;
  using TReader = itk::ImageFileReader<TImage>;
  using TExtract = itk::ExtractConnectedComponentsImageFilter<TImage>;
  using TWriter = itk::ImageFileWriter<TImage>;

  const auto reader = TReader::New();
  reader->SetFileName(IImage);

  const auto extract = TExtract::New();
  extract->SetInput( reader->GetOutput() );
  extract->SetN( N );

  const auto writer = TWriter::New();
  writer->SetInput(extract->GetOutput());
  writer->SetFileName(OImage);
  writer->Update();
}

}

#endif
