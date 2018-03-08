#ifndef dv_ImageToSet_h
#define dv_ImageToSet_h

#include <itkImage.h>
#include <itkImageRegionConstIterator.h>
#include <set>

namespace dv
{

template<unsigned int Dimension, typename TPixel>
std::set< TPixel >
ImageToSet( const typename itk::Image< TPixel, Dimension >::Pointer image )
{

  std::set< TPixel > labels;
  itk::ImageRegionConstIterator< typename itk::Image< TPixel, Dimension > > it( image, image->GetLargestPossibleRegion() );

  for ( it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
    labels.emplace( it.Get() );
    }

  return labels;

}

}

#endif
