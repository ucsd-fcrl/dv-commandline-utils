#ifndef dv_AnisotropicSmoothing_h
#define dv_AnisotropicSmoothing_h

// ITK
#include <itkGradientAnisotropicDiffusionImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

namespace dv {

template<unsigned int Dimension, typename TPixel>
void
AnisotropicSmoothing(const std::string& IImage,
                     const std::string& OImage,
                     const unsigned int Iterations,
                     const double TimeStep,
                     const double Conductance)
{

  using TImage = itk::Image<TPixel, Dimension>;
  using TReader = itk::ImageFileReader<TImage>;
  using TFilter = itk::GradientAnisotropicDiffusionImageFilter<TImage, TImage>;
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
  filter->SetInput(reader->GetOutput());
  filter->SetNumberOfIterations(Iterations);
  filter->SetTimeStep(TimeStep);
  filter->SetConductanceParameter(Conductance);
  filter->UseImageSpacingOn();

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
