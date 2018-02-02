#ifndef dv_ResampleFromReference_h
#define dv_ResampleFromReference_h

#include <itkImage.h>
#include <itkIdentityTransform.h>
#include <itkResampleImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

namespace dv
{

template<unsigned int Dimension, typename PixelType>
void
ResampleFromReference(
  const std::string iImageFileName,
  const std::string rImageFileName,
  const std::string oImageFileName,
  const double outsideValue)
{

  using ImageType     = itk::Image< PixelType, Dimension >;
  using ReaderType    = itk::ImageFileReader< ImageType >;
  using WriterType    = itk::ImageFileWriter< ImageType >;
  using TransformType = itk::IdentityTransform< double, Dimension >;
  using ResampleType  = itk::ResampleImageFilter< ImageType, ImageType >;

  const auto iReader = ReaderType::New();
  const auto rReader = ReaderType::New();
  const auto resample = ResampleType::New();
  const auto writer = WriterType::New();

  iReader->SetFileName( iImageFileName );
  rReader->SetFileName( rImageFileName );

  resample->SetInput( iReader->GetOutput() );
  resample->SetReferenceImage( rReader->GetOutput() );
  resample->UseReferenceImageOn();
  resample->SetTransform( TransformType::New() );
  resample->SetDefaultPixelValue( outsideValue );

  writer->SetInput( resample->GetOutput() );
  writer->SetFileName( oImageFileName );
  writer->Update();

}

}

#endif
