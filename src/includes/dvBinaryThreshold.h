#ifndef dv_BinaryThreshold_h
#define dv_BinaryThreshold_h

// ITK
#include <itkImageFileReader.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkImageFileWriter.h>

namespace dv
{

template<unsigned int Dimension, typename TPixel>
void
BinaryThreshold(po::variables_map vm)
{

  using TImage = itk::Image<TPixel, Dimension>;
  using TReader = itk::ImageFileReader<TImage>;
  using TFilter = itk::BinaryThresholdImageFilter<TImage, TImage>;
  using TWriter = itk::ImageFileWriter<TImage>;
 
  const auto reader = TReader::New();
  reader->SetFileName( vm["input-image"].as<std::string>() );
 
  const auto filter = TFilter::New();
  filter->SetInput(reader->GetOutput());

  if (vm.count("interior-value"))
    {
    filter->SetInsideValue(vm["interior-value"].as<TPixel>());
    }

  if (vm.count("exterior-value"))
    {
    filter->SetOutsideValue(vm["exterior-value"].as<TPixel>());
    }

  if (vm.count("lower-threshold"))
    {
    filter->SetLowerThreshold(vm["lower-threshold"].as<TPixel>());
    }

  if (vm.count("upper-threshold"))
    {
    filter->SetUpperThreshold(vm["upper-threshold"].as<TPixel>());
    }

  const auto writer = TWriter::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( vm["output-image"].as<std::string>() );
  writer->Update();

}

}

#endif
