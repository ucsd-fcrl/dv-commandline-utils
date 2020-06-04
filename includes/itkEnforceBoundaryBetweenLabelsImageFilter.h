#ifndef itkEnforceBoundaryBetweenLabelsImageFilter_h
#define itkEnforceBoundaryBetweenLabelsImageFilter_h

#include "itkImageToImageFilter.h"

namespace itk
{

/** \class EnforceBoundaryBetweenLabelsImageFilter
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
class EnforceBoundaryBetweenLabelsImageFilter : public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  ITK_DISALLOW_COPY_AND_ASSIGN(EnforceBoundaryBetweenLabelsImageFilter);

  static constexpr unsigned int InputImageDimension = TInputImage::ImageDimension;
  static constexpr unsigned int OutputImageDimension = TOutputImage::ImageDimension;

  using InputImageType = TInputImage;
  using OutputImageType = TOutputImage;
  using InputPixelType = typename InputImageType::PixelType;
  using OutputPixelType = typename OutputImageType::PixelType;
  using LabelSetType = std::set<InputPixelType>;

  /** Standard class typedefs. */
  using Self = EnforceBoundaryBetweenLabelsImageFilter<InputImageType, OutputImageType>;
  using Superclass = ImageToImageFilter<InputImageType, OutputImageType>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Run-time type information. */
  itkTypeMacro(EnforceBoundaryBetweenLabelsImageFilter, ImageToImageFilter);

  /** Standard New macro. */
  itkNewMacro(Self);

  void SetLabels1(const LabelSetType _arg)
  {
    this->m_Labels1 = _arg;
  }
  itkGetConstMacro(Labels1, LabelSetType);

  void SetLabels2(const LabelSetType _arg)
  {
    this->m_Labels2 = _arg;
  }
  itkGetConstMacro(Labels2, LabelSetType);

  itkSetMacro(Radius1, unsigned int);
  itkGetConstMacro(Radius1, unsigned int);

  itkSetMacro(Radius2, unsigned int);
  itkGetConstMacro(Radius2, unsigned int);

  itkSetMacro(Replacement, InputPixelType);
  itkGetConstMacro(Replacement, InputPixelType);
protected:
  EnforceBoundaryBetweenLabelsImageFilter();
  ~EnforceBoundaryBetweenLabelsImageFilter() override = default;

  void PrintSelf(std::ostream & os, Indent indent) const override;

  using OutputRegionType = typename OutputImageType::RegionType;

  void GenerateData() override;

private:
#ifdef ITK_USE_CONCEPT_CHECKING
  // Add concept checking such as
  // itkConceptMacro( FloatingPointPixel, ( itk::Concept::IsFloatingPoint< typename InputImageType::PixelType > ) );
#endif


LabelSetType m_Labels1;
LabelSetType m_Labels2;
unsigned int m_Radius1;
unsigned int m_Radius2;
InputPixelType m_Replacement;

};
} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkEnforceBoundaryBetweenLabelsImageFilter.hxx"
#endif

#endif // itkEnforceBoundaryBetweenLabelsImageFilter
