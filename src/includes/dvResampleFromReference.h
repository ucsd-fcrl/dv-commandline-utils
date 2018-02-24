#ifndef dv_ResampleFromReference_h
#define dv_ResampleFromReference_h

#include <itkImage.h>
#include <itkIdentityTransform.h>
#include <itkResampleImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

namespace dv
{

template<unsigned int Dimension, typename TPixel>
void
ResampleFromReference(
  const std::string IImage,
  const std::string RImage,
  const std::string OImage,
  const double OutsideValue,
  const bool OutputSizeExists,
  const unsigned int OutputSize,
  const bool OutputSpacingExists,
  const double OutputSpacing
)
{

  using TImage     = itk::Image< TPixel, Dimension >;
  using TReader    = itk::ImageFileReader< TImage >;
  using TWriter    = itk::ImageFileWriter< TImage >;
  using TTransform = itk::IdentityTransform< double, Dimension >;
  using TResample  = itk::ResampleImageFilter< TImage, TImage >;

  const auto iReader = TReader::New();
  const auto rReader = TReader::New();
  iReader->SetFileName( IImage );
  rReader->SetFileName( RImage );
  rReader->Update();

  const auto resample = TResample::New();
  resample->SetInput( iReader->GetOutput() );
  resample->SetTransform( TTransform::New() );
  resample->SetDefaultPixelValue( OutsideValue );

  // Default is to use the reference image.
  const auto ReferenceSize = rReader->GetOutput()->GetLargestPossibleRegion().GetSize();
  const auto ReferenceSpacing = rReader->GetOutput()->GetSpacing();
  const auto ReferenceDirection = rReader->GetOutput()->GetDirection();
  const auto ReferenceOrigin = rReader->GetOutput()->GetOrigin();

  if (OutputSpacingExists && OutputSizeExists)
    {
    // Set Spacing
    const double TempOutputSpacing[3] = {OutputSpacing, OutputSpacing, 1};
    resample->SetOutputSpacing( TempOutputSpacing );

    // Set Size
    const itk::Size<Dimension> TempOutputSize = {{OutputSize, OutputSize, 1}};
    resample->SetSize( TempOutputSize );

    // Set Origin
    itk::Vector<double,3> ReferenceCenter;
    ReferenceCenter[0] = ( ReferenceSize[0] / 2.0 ) * ( ReferenceSpacing[0] );
    ReferenceCenter[1] = ( ReferenceSize[1] / 2.0 ) * ( ReferenceSpacing[1] );
    ReferenceCenter[2] = 0.0;

    itk::Vector<double,3> UserCenter;
    UserCenter[0] = OutputSize / 2.0 * OutputSpacing;
    UserCenter[1] = OutputSize / 2.0 * OutputSpacing;
    UserCenter[2] = 0.0;

    const auto CenterDiff = ReferenceCenter - UserCenter;

    const auto OriginShift = ReferenceDirection * CenterDiff;

    const auto NewOrigin = ReferenceOrigin + OriginShift;

    resample->SetOutputOrigin( NewOrigin );
    }
  else
    {
    resample->SetOutputOrigin( ReferenceOrigin );
    resample->SetOutputSpacing( ReferenceSpacing );
    resample->SetOutputDirection( ReferenceDirection );
    resample->SetSize( ReferenceSize );
    }


  const auto writer = TWriter::New();
  writer->SetInput( resample->GetOutput() );
  writer->SetFileName( OImage );
  writer->Update();

}

}

#endif
