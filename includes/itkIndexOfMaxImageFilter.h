#ifndef itk_IndexOfMaxImageFilter_h
#define itk_IndexOfMaxImageFilter_h

#include <itkImage.h>
#include <itkUnaryFunctorImageFilter.h>

namespace itk
{

  namespace Functor
  {

    template< typename TInput, typename TOutput = TInput>
    class IndexOfMaxFunctor
    {
    public:
      
      IndexOfMaxFunctor() {};
      ~IndexOfMaxFunctor() {};

      /**
       * Returns index + 1 (label) of maximum value greater than 0.5, otherwise 0
       * TInput - input image PixelType (vector)
       * TOutput - output image PixelType (int)
       **/
      inline TOutput operator()( const TInput & A )
      {

        int maxIndex = 0;
        double maxVal = 0;
        for (size_t i = 0; i < A.Size(); i++) {
          if (A.GetElement(i) > maxVal && A.GetElement(i) > 0.5) {
            maxVal = A.GetElement(i);
            maxIndex = i + 1;
          }
        }
        return static_cast<TOutput>( maxIndex );
      }

    };
  };

  template<typename TInputImage, typename TOutputImage>
  class IndexOfMaxImageFilter
    : public UnaryFunctorImageFilter<
        TInputImage,
        TOutputImage,
        Functor::IndexOfMaxFunctor< typename TInputImage::PixelType, typename TOutputImage::PixelType> >
  {
  public:

    using Self = IndexOfMaxImageFilter;
    using Superclass = UnaryFunctorImageFilter< TInputImage,
                                                TOutputImage,
                                                Functor::IndexOfMaxFunctor< typename TInputImage::PixelType, typename TOutputImage::PixelType > >;

    using Pointer = SmartPointer< Self >;
    using ConstPointer = SmartPointer< const Self >;

    itkNewMacro( Self );
    itkTypeMacro( IndexOfMaxImageFilter, UnaryFunctorImageFilter );

  protected:

    IndexOfMaxImageFilter() {}
    virtual ~IndexOfMaxImageFilter() {}

  private:

    IndexOfMaxImageFilter( const Self & ); // purposely not implemented
    void operator=(  const Self & );      // purposely not implemented

  };   // end of class
};

#endif
