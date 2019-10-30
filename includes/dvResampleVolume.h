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
#include <itkIndexOfMaxImageFilter.h>

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
  using ReaderType = itk::ImageFileReader<ImageType>;
  using WriterType = itk::ImageFileWriter<ImageType>;
  using TransformType = itk::IdentityTransform<float, Dimension>;
  using ResampleType = itk::ResampleImageFilter<ImageType, ImageType, float, float>;

  // Image and filter types for linear resampling segmentation by label
  using ImageTypeML = itk::Image<float, Dimension>;
  using ResampleTypeML = itk::ResampleImageFilter<ImageType, ImageTypeML, float, float>;
 
  using VectorImageType = itk::VectorImage<float, Dimension>;
  using ChangeLabelsFilterType = itk::ChangeLabelsImageFilter<ImageType>;
  using ImageToVectorImageFilterType = itk::ComposeImageFilter<ImageTypeML>;
  using MaxFilterType = itk::IndexOfMaxImageFilter<VectorImageType, ImageType>;

  // Interpolator types
  using NNInterpolateType = itk::NearestNeighborInterpolateImageFunction<ImageType, float>;
  using LNInterpolateType = itk::LinearInterpolateImageFunction<ImageType, float>;
  using BSInterpolateType = itk::BSplineInterpolateImageFunction<ImageType, float>;

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

  // create resample filter
  auto resample = ResampleType::New();;
  if (interpolator != 2) {
    resample->SetTransform(TransformType::New());
    resample->SetOutputStartIndex(image->GetLargestPossibleRegion().GetIndex());
    resample->SetOutputOrigin(image->GetOrigin());
    resample->SetOutputDirection(image->GetDirection());
    resample->SetOutputSpacing(outputSpacing);
    resample->SetSize(outputSize);
    resample->SetInput(image);
  }
  

  switch (interpolator)
  {
  case 0: // Nearest Neighbor
  {
    const auto interp = NNInterpolateType::New();
    resample->SetInterpolator(interp);
    writer->SetInput(resample->GetOutput());

    break;
  }
  case 1: // Linear
  {
    const auto interp = LNInterpolateType::New();
    resample->SetInterpolator(interp);
    writer->SetInput(resample->GetOutput());

    break;
  }
  case 2: // Linear by segmentation label
  {

    // linearly interpolate labels separately
    const auto interp = LNInterpolateType::New();
    const auto imageToVectorImageFilter = ImageToVectorImageFilterType::New();

    // create label map (all map to 0)
    const auto labels_set = dv::ImageToSet<Dimension, TPixel>(image);
    std::map<TPixel, TPixel> label_map;
    for (const auto &x : labels_set)
    {
      int s = (int)x;
      label_map[s] = 0;
    }

    int layer = 0;
    for (const auto &x : labels_set)
    {
      int s = (int)x;

      // Ignore background
      if (s == 0)
        continue;

      // change current label s to map to 1 in label_map
      label_map[s] = 1;

      const auto filter = ChangeLabelsFilterType::New();
      filter->SetLabelMap(label_map);
      filter->SetInput(reader->GetOutput());

      auto resample = ResampleTypeML::New();
      resample->SetTransform(TransformType::New());
      resample->SetOutputStartIndex(image->GetLargestPossibleRegion().GetIndex());
      resample->SetOutputOrigin(image->GetOrigin());
      resample->SetOutputDirection(image->GetDirection());
      resample->SetOutputSpacing(outputSpacing);
      resample->SetSize(outputSize);
      resample->SetInput(filter->GetOutput());
      resample->SetInterpolator(interp);
      resample->Update();

      // Add output of resample to composite filter
      imageToVectorImageFilter->SetInput(layer, resample->GetOutput());
      imageToVectorImageFilter->Update();

      // revert label_map
      label_map[s] = 0;
      layer++;
    }

    // get the max of each vector in VectorImage output from imageToVectorImageFilter
    const auto maxFilter = MaxFilterType::New();
    maxFilter->SetInput(imageToVectorImageFilter->GetOutput());
    maxFilter->Update();

    writer->SetInput(maxFilter->GetOutput());

    break;
  }
  default: // BSpline
  {
    const auto interp = BSInterpolateType::New();
    interp->SetSplineOrder(interpolator);
    resample->SetInterpolator(interp);
    writer->SetInput(resample->GetOutput());

    break;
  }
  }
  
  // write file
  writer->SetFileName(OImage);
  writer->Update();
}
} // namespace dv
#endif