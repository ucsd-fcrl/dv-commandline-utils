#ifndef itkFillHolesInSegmentationImageFilter_h
#define itkFillHolesInSegmentationImageFilter_h

#include "itkImageToImageFilter.h"

namespace itk
{

/** \class FillHolesInSegmentationImageFilter
 *
 * \brief Filters a image by iterating over its pixels.
 *
 * Filters a image by iterating over its pixels in a multi-threaded way
 * and {to be completed by the developer}.
 *
 * \ingroup DVUtils
 *
 */
template <typename TInputImage, typename TOutputImage = TInputImage>
class FillHolesInSegmentationImageFilter : public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  ITK_DISALLOW_COPY_AND_ASSIGN(FillHolesInSegmentationImageFilter);

  static constexpr unsigned int InputImageDimension = TInputImage::ImageDimension;
  static constexpr unsigned int OutputImageDimension = TOutputImage::ImageDimension;

  using InputImageType = TInputImage;
  using OutputImageType = TOutputImage;
  using InputPixelType = typename InputImageType::PixelType;
  using OutputPixelType = typename OutputImageType::PixelType;

  /** Standard class typedefs. */
  using Self = FillHolesInSegmentationImageFilter<InputImageType, OutputImageType>;
  using Superclass = ImageToImageFilter<InputImageType, OutputImageType>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Run-time type information. */
  itkTypeMacro(FillHolesInSegmentationImageFilter, ImageToImageFilter);

  /** Standard New macro. */
  itkNewMacro(Self);

protected:
  FillHolesInSegmentationImageFilter();
  ~FillHolesInSegmentationImageFilter() override = default;

  void PrintSelf(std::ostream & os, Indent indent) const override;

  using OutputRegionType = typename OutputImageType::RegionType;

  void GenerateData() override;

private:
#ifdef ITK_USE_CONCEPT_CHECKING
  // Add concept checking such as
  // itkConceptMacro( FloatingPointPixel, ( itk::Concept::IsFloatingPoint< typename InputImageType::PixelType > ) );
#endif
};
} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkFillHolesInSegmentationImageFilter.hxx"
#endif

#endif // itkFillHolesInSegmentationImageFilter
