#ifndef itk_ChangeLabelsImageFilter_h
#define itk_ChangeLabelsImageFilter_h

#include <itkImage.h>
#include <itkUnaryFunctorImageFilter.h>

#include <map>

namespace itk {

namespace Functor {

template<typename TInput, typename TOutput = TInput>
class ChangeLabelsFunctor
{
public:
  using TMap = std::map<TInput, TInput>;

  ChangeLabelsFunctor(){};
  ~ChangeLabelsFunctor(){};

  inline TOutput operator()(const TInput& A)
  {
    if (this->functLabels.find(A) == this->functLabels.cend()) {
      return static_cast<TOutput>(A);
    } else {
      return static_cast<TOutput>(this->functLabels[A]);
    }
  }

  void SetLabelMap(TMap value) { functLabels = value; }

private:
  TMap functLabels;
};
};

template<typename TInputImage, typename TOutputImage = TInputImage>
class ChangeLabelsImageFilter
  : public UnaryFunctorImageFilter<
      TInputImage,
      TOutputImage,
      Functor::ChangeLabelsFunctor<typename TInputImage::PixelType,
                                   typename TOutputImage::PixelType>>
{
public:
  using Self = ChangeLabelsImageFilter;
  using Superclass = UnaryFunctorImageFilter<
    TInputImage,
    TOutputImage,
    Functor::ChangeLabelsFunctor<typename TInputImage::PixelType,
                                 typename TOutputImage::PixelType>>;

  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  itkNewMacro(Self);
  itkTypeMacro(ChangeLabelsImageFilter, UnaryFunctorImageFilter);

  void SetLabelMap(std::map<typename TInputImage::PixelType,
                            typename TInputImage::PixelType> value)
  {
    this->GetFunctor().SetLabelMap(value);
  }

protected:
  ChangeLabelsImageFilter() {}
  virtual ~ChangeLabelsImageFilter() {}

private:
  ChangeLabelsImageFilter(const Self&); // purposely not implemented
  void operator=(const Self&);          // purposely not implemented

}; // end of class
};

#endif
