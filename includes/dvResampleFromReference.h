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

namespace dv {

template<class TImage,typename TCoordRep, unsigned int SplineOrder> class BSplineInterpolator : public itk::BSplineInterpolateImageFunction<TImage,TCoordRep> {
  public:
  /** Standard class type aliases. */
  using Self = BSplineInterpolator;
  using Superclass =
    itk::BSplineInterpolateImageFunction<TImage,TCoordRep>;
  using Pointer = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(Self, Superclass);

  BSplineInterpolator() {
    this->SetSplineOrder( SplineOrder );
  }

};

template<class TImage, typename TCoordRep> class BSplineInterpolator2 : public BSplineInterpolator<TImage, TCoordRep, 2>{};
template<class TImage, typename TCoordRep> class BSplineInterpolator3 : public BSplineInterpolator<TImage, TCoordRep, 3>{};
template<class TImage, typename TCoordRep> class BSplineInterpolator4 : public BSplineInterpolator<TImage, TCoordRep, 4>{};
template<class TImage, typename TCoordRep> class BSplineInterpolator5 : public BSplineInterpolator<TImage, TCoordRep, 5>{};

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
  using GBSInterpolateType2 =
    itk::LabelImageGenericInterpolateImageFunction<TImage,BSplineInterpolator2>;
  using GBSInterpolateType3 =
    itk::LabelImageGenericInterpolateImageFunction<TImage,BSplineInterpolator3>;
  using GBSInterpolateType4 =
    itk::LabelImageGenericInterpolateImageFunction<TImage,BSplineInterpolator4>;
  using GBSInterpolateType5 =
    itk::LabelImageGenericInterpolateImageFunction<TImage,BSplineInterpolator5>;

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
    case 2: {
      if (voting) {
        const auto interp = GBSInterpolateType2::New();
        resample->SetInterpolator(interp);
      } else {
        const auto interp = BSInterpolateType::New();
        interp->SetSplineOrder(interpolator);
        resample->SetInterpolator(interp);
      }
      break;
    }
    case 3: {
      if (voting) {
        const auto interp = GBSInterpolateType3::New();
        resample->SetInterpolator(interp);
      } else {
        const auto interp = BSInterpolateType::New();
        interp->SetSplineOrder(interpolator);
        resample->SetInterpolator(interp);
      }
      break;
    }
    case 4: {
      if (voting) {
        const auto interp = GBSInterpolateType4::New();
        resample->SetInterpolator(interp);
      } else {
        const auto interp = BSInterpolateType::New();
        interp->SetSplineOrder(interpolator);
        resample->SetInterpolator(interp);
      }
      break;
    }
    case 5: {
      if (voting) {
        const auto interp = GBSInterpolateType5::New();
        resample->SetInterpolator(interp);
      } else {
        const auto interp = BSInterpolateType::New();
        interp->SetSplineOrder(interpolator);
        resample->SetInterpolator(interp);
      }
      break;
    }
    default: {
      itkAssertOrThrowMacro(false, "This interpolator is not supported.");
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
