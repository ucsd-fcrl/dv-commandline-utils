#ifndef dv_ResampleFromReference_h
#define dv_ResampleFromReference_h

#include <itkIdentityTransform.h>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkResampleImageFilter.h>

#include <itkBSplineInterpolateImageFunction.h>
#include <itkLinearInterpolateImageFunction.h>
#include <itkNearestNeighborInterpolateImageFunction.h>
#include <itkLabelImageGenericInterpolateImageFunction.h>

template<class TImage,typename TCoordRep> class ReducedBSplineInterpolateImageFunction : public itk::BSplineInterpolateImageFunction<TImage,TCoordRep> {};

namespace dv {

template<unsigned int Dimension, typename TPixel>
void
ResampleFromReference(const std::string IImage,
                      const std::string RImage,
                      const std::string OImage,
                      const double OutsideValue,
                      const bool OutputSizeExists,
                      const unsigned int OutputSize,
                      const bool OutputSpacingExists,
                      const double OutputSpacing,
                      const unsigned int& interpolator,
                      const bool voting)
{

  using TImage = itk::Image<TPixel, Dimension>;
  using TReader = itk::ImageFileReader<TImage>;
  using TWriter = itk::ImageFileWriter<TImage>;
  using TTransform = itk::IdentityTransform<double, Dimension>;
  using TResample = itk::ResampleImageFilter<TImage, TImage>;

  using NNInterpolateType =
    itk::NearestNeighborInterpolateImageFunction<TImage>;
  using LNInterpolateType = itk::LinearInterpolateImageFunction<TImage>;
  using BSInterpolateType = itk::BSplineInterpolateImageFunction<TImage>;

  using GNNInterpolateType =
    itk::LabelImageGenericInterpolateImageFunction<TImage,itk::NearestNeighborInterpolateImageFunction>;
  using GLNInterpolateType =
    itk::LabelImageGenericInterpolateImageFunction<TImage,itk::LinearInterpolateImageFunction>;
  using GBSInterpolateType =
    itk::LabelImageGenericInterpolateImageFunction<TImage,ReducedBSplineInterpolateImageFunction>;

  const auto iReader = TReader::New();
  const auto rReader = TReader::New();
  iReader->SetFileName(IImage);
  iReader->Update();
  rReader->SetFileName(RImage);
  rReader->Update();

  const auto resample = TResample::New();
  resample->SetInput(iReader->GetOutput());
  resample->SetTransform(TTransform::New());
  resample->SetDefaultPixelValue(OutsideValue);

  // Default is to use the reference image.
  const auto ReferenceSize =
    rReader->GetOutput()->GetLargestPossibleRegion().GetSize();
  const auto ReferenceSpacing = rReader->GetOutput()->GetSpacing();
  const auto ReferenceDirection = rReader->GetOutput()->GetDirection();
  const auto ReferenceOrigin = rReader->GetOutput()->GetOrigin();

  if (OutputSpacingExists && OutputSizeExists) {
    // Set Spacing
    const double TempOutputSpacing[3] = { OutputSpacing, OutputSpacing, 1 };
    resample->SetOutputSpacing(TempOutputSpacing);

    // Set Size
    const itk::Size<Dimension> TempOutputSize = {
      { OutputSize, OutputSize, 1 }
    };
    resample->SetSize(TempOutputSize);

    // Set Origin
    itk::Vector<double, 3> ReferenceCenter;
    ReferenceCenter[0] = (ReferenceSize[0] / 2.0) * (ReferenceSpacing[0]);
    ReferenceCenter[1] = (ReferenceSize[1] / 2.0) * (ReferenceSpacing[1]);
    ReferenceCenter[2] = 0.0;

    itk::Vector<double, 3> UserCenter;
    UserCenter[0] = OutputSize / 2.0 * OutputSpacing;
    UserCenter[1] = OutputSize / 2.0 * OutputSpacing;
    UserCenter[2] = 0.0;

    const auto CenterDiff = ReferenceCenter - UserCenter;

    const auto OriginShift = ReferenceDirection * CenterDiff;

    const auto NewOrigin = ReferenceOrigin + OriginShift;

    resample->SetOutputOrigin(NewOrigin);
  } else {
    resample->SetOutputOrigin(ReferenceOrigin);
    resample->SetOutputSpacing(ReferenceSpacing);
    resample->SetSize(ReferenceSize);
  }

  resample->SetOutputDirection(ReferenceDirection);
  switch (interpolator) {
    case 0: {
      if (voting) {
        const auto interp = GNNInterpolateType::New();
        resample->SetInterpolator(interp);
      } else {
        const auto interp = NNInterpolateType::New();
        resample->SetInterpolator(interp);
      }
      break;
    }
    case 1: {
      if (voting) {
        const auto interp = GLNInterpolateType::New();
        resample->SetInterpolator(interp);
      } else {
        const auto interp = LNInterpolateType::New();
        resample->SetInterpolator(interp);
      }
      break;
    }
    default: {
      if (voting) {
        const auto interp = GBSInterpolateType::New();
// TODO: Setting spline order is not currently supported.
// https://discourse.itk.org/t/labelimagegenericinterpolateimagefunction-and-bsplineinterpolateimagefunction/2668
//        interp->SetSplineOrder(interpolator);
        resample->SetInterpolator(interp);
      } else {
        const auto interp = BSInterpolateType::New();
        interp->SetSplineOrder(interpolator);
        resample->SetInterpolator(interp);
      }
      break;
    }
  }

  const auto writer = TWriter::New();
  writer->SetInput(resample->GetOutput());
  writer->SetFileName(OImage);
  writer->Update();
}

}

#endif
