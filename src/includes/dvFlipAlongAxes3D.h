#ifndef dv_FlipAlongAxes3D_h
#define dv_FlipAlongAxes3D_h

// ITK
#include <itkImageFileReader.h>
#include <itkFlipImageFilter.h>
#include <itkImageFileWriter.h>

namespace dv
{

template<unsigned int Dimension, typename TPixel>
void
FlipAlongAxes3D(const std::string &IImage,
                const std::string &OImage,
                const itk::FixedArray<unsigned int, Dimension> &order)
{
 
  using TImage = itk::Image<TPixel, Dimension>;

  using TReader = itk::ImageFileReader<TImage>;
  using TFlip   = itk::FlipImageFilter<TImage>;
  using TWriter = itk::ImageFileWriter<TImage>;

  const auto reader = TReader::New();
  reader->SetFileName( IImage );
  
  const auto flip = TFlip::New();
  flip->SetInput( reader->GetOutput() );
  flip->SetFlipAxes(order);

  const auto writer = TWriter::New();
  writer->SetInput( flip->GetOutput() );
  writer->SetFileName( OImage );
  writer->Update();

} 

}

#endif
