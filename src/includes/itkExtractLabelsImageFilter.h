#ifndef itk_ExtractLabelsImageFilter_h
#define itk_ExtractLabelsImageFilter_h

#include <itkImage.h>
#include <itkUnaryFunctorImageFilter.h>

#include <set>

namespace itk
{

  namespace Functor
  {

    template< typename TInput, typename TOutput = TInput>
    class ExtractLabelsFunctor
    {
    public:
      ExtractLabelsFunctor() {};
      ~ExtractLabelsFunctor() {};

      inline TOutput operator()( const TInput & A )
      {
        // Checks the functLabels for the value. Returns 1 if included. 0 otherwise
        return static_cast<TOutput>( functLabels.count(A) );
      }

      void SetLabels( std::set<TInput> value )
      {
        functLabels = value;
      }

    private:
      std::set<TInput>  functLabels;

    };
  };

  template<typename TInputImage, typename TOutputImage = TInputImage>
  class ExtractLabelsImageFilter :
  public UnaryFunctorImageFilter< TInputImage,TOutputImage,
  Functor::ExtractLabelsFunctor< typename TInputImage::PixelType, typename TOutputImage::PixelType> >
  {
  public:

    using Self = ExtractLabelsImageFilter;
    using Superclass = UnaryFunctorImageFilter< TInputImage,
                                                TOutputImage,
                                                Functor::ExtractLabelsFunctor< typename TInputImage::PixelType, typename TOutputImage::PixelType > >;

    using Pointer = SmartPointer< Self >;
    using ConstPointer = SmartPointer< const Self >;

    itkNewMacro( Self );
    itkTypeMacro( ExtractLabelsImageFilter, UnaryFunctorImageFilter );

    void SetLabels( std::set<typename TInputImage::PixelType> value )
    {
      this->GetFunctor().SetLabels( value );
    }

  protected:

    ExtractLabelsImageFilter() {}
    virtual ~ExtractLabelsImageFilter() {}

  private:

    ExtractLabelsImageFilter( const Self & ); // purposely not implemented
    void operator=(  const Self & );      // purposely not implemented

  };   // end of class
};

#endif
