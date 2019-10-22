#ifndef dv_ResampleVolume_h
#define dv_ResampleVolume_h

// ITK
#include <itkImage.h>
#include <itkIdentityTransform.h>
#include <itkResampleImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkFileTools.h>

#include <itkNearestNeighborInterpolateImageFunction.h>
#include <itkLinearInterpolateImageFunction.h>
#include <itkBSplineInterpolateImageFunction.h>

#include <itkChangeLabelsImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkImageFileWriter.h>
#include <itkComposeImageFilter.h>

#include <itkVectorImage.h>
// #include <itkImageAlgorithm.h>
#include <itkIndexOfMaxImageFilter.h>
// #include <itkSmartPointer.h>

#include <dvImageToSet.h>

namespace dv
{

template <unsigned int Dimension, typename TPixel>
void ResampleVolume(const std::string &IImage,
                    const std::string &OImage,
                    const double &spacing,
                    const unsigned int &interpolator)
{
  using ImageType = itk::Image<TPixel, Dimension>;
  using ImageTypeML = itk::Image<double, Dimension>;
  using VectorImageType = itk::VectorImage<double, Dimension>;
  using ReaderType = itk::ImageFileReader<ImageType>;
  using WriterType = itk::ImageFileWriter<ImageType>;
  using WriterTypeML = itk::ImageFileWriter<ImageTypeML>;
  using TransformType = itk::IdentityTransform<double, Dimension>;
  using ResampleType = itk::ResampleImageFilter<ImageType, ImageType>;
  using ResampleTypeML = itk::ResampleImageFilter<ImageType, ImageTypeML>;
  using ChangeLabelsFilterType = itk::ChangeLabelsImageFilter<ImageType>;
  using ImageToVectorImageFilterType = itk::ComposeImageFilter<ImageTypeML>;
  using MaxFilterType = itk::IndexOfMaxImageFilter<VectorImageType, ImageType>;

  using NNInterpolateType = itk::NearestNeighborInterpolateImageFunction<ImageType>;
  using LNInterpolateType = itk::LinearInterpolateImageFunction<ImageType>;
  using BSInterpolateType = itk::BSplineInterpolateImageFunction<ImageType>;

  const auto reader = ReaderType::New();
  const auto writer = WriterType::New();

  // Read input image file
  reader->SetFileName(IImage);
  reader->Update();

  // Graft onto an ImageType
  const auto image = ImageType::New();
  image->Graft(reader->GetOutput());

  // Create ImageType for resampled output
  typename ImageType::SpacingType outputSpacing;
  outputSpacing.Fill(spacing);
  auto inputSize = image->GetLargestPossibleRegion().GetSize();
  auto inputSpacing = image->GetSpacing();
  typename ImageType::SizeType outputSize;
  for (unsigned int i = 0; i < Dimension; ++i)
  {
    outputSize[i] = std::floor(double(inputSize[i]) * inputSpacing[i] / outputSpacing[i]);
  }

  switch (interpolator)
  {
  case 0:
  {
    const auto resample = ResampleType::New();
    resample->SetTransform(TransformType::New());
    resample->SetOutputStartIndex(image->GetLargestPossibleRegion().GetIndex());
    resample->SetOutputOrigin(image->GetOrigin());
    resample->SetOutputDirection(image->GetDirection());
    resample->SetOutputSpacing(outputSpacing);
    resample->SetSize(outputSize);
    resample->SetInput(image);

    const auto interp = NNInterpolateType::New();
    resample->SetInterpolator(interp);

    writer->SetInput(resample->GetOutput());
    writer->SetFileName(OImage);
    writer->Update();

    break;
  }
  case 1:
  {
    const auto resample = ResampleType::New();
    resample->SetTransform(TransformType::New());
    resample->SetOutputStartIndex(image->GetLargestPossibleRegion().GetIndex());
    resample->SetOutputOrigin(image->GetOrigin());
    resample->SetOutputDirection(image->GetDirection());
    resample->SetOutputSpacing(outputSpacing);
    resample->SetSize(outputSize);
    resample->SetInput(image);

    const auto interp = LNInterpolateType::New();
    resample->SetInterpolator(interp);

    writer->SetInput(resample->GetOutput());
    writer->SetFileName(OImage);
    writer->Update();

    break;
  }
  case 2:
  {
    const auto writerML = WriterTypeML::New();

    // linearly interpolate classes separately
    const auto interp = LNInterpolateType::New();
    const auto imageToVectorImageFilter = ImageToVectorImageFilterType::New();

    // labels set
    const auto labels_set = dv::ImageToSet<Dimension, TPixel>(image);
    std::map<TPixel, TPixel> label_map;
    for (const auto &x : labels_set)
    {
      int s = (int)x;
      label_map[s] = 0;
    }

    for (const auto &x : labels_set)
    {
      int s = (int)x;

      // Ignore background.
      if (s == 0)
        continue;

      label_map[s] = 1;

      const auto filter = ChangeLabelsFilterType::New();
      filter->SetLabelMap(label_map);
      filter->SetInput(reader->GetOutput());

      const auto resampleML = ResampleTypeML::New();
      resampleML->SetTransform(TransformType::New());
      resampleML->SetOutputStartIndex(image->GetLargestPossibleRegion().GetIndex());
      resampleML->SetOutputOrigin(image->GetOrigin());
      resampleML->SetOutputDirection(image->GetDirection());
      resampleML->SetOutputSpacing(outputSpacing);
      resampleML->SetSize(outputSize);
      resampleML->SetInput(filter->GetOutput());
      resampleML->SetInterpolator(interp);
      resampleML->Update();

      // Add output of resample to composite filter
      imageToVectorImageFilter->SetInput(s - 1, resampleML->GetOutput());
      imageToVectorImageFilter->Update();

      label_map[s] = 0;
    }

    // get the max of each vector in VectorImage output from imageToVectorImageFilter
    const auto maxFilter = MaxFilterType::New();
    maxFilter->SetInput(imageToVectorImageFilter->GetOutput());
    maxFilter->Update();

    writer->SetInput(maxFilter->GetOutput());
    writer->SetFileName(OImage);
    writer->Update();
    break;
  }
  default:
  {
    const auto resample = ResampleType::New();
    resample->SetTransform(TransformType::New());
    resample->SetOutputStartIndex(image->GetLargestPossibleRegion().GetIndex());
    resample->SetOutputOrigin(image->GetOrigin());
    resample->SetOutputDirection(image->GetDirection());
    resample->SetOutputSpacing(outputSpacing);
    resample->SetSize(outputSize);
    resample->SetInput(image);

    const auto interp = BSInterpolateType::New();
    interp->SetSplineOrder(interpolator);
    resample->SetInterpolator(interp);
    
    writer->SetInput(resample->GetOutput());
    writer->SetFileName(OImage);
    writer->Update();
    break;
  }
  }
}

} // namespace dv
#endif