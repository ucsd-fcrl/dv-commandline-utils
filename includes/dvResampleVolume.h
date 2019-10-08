#ifndef dv_ResampleVolume_h
#define dv_ResampleVolume_h

// ITK
#include <itkImage.h>
#include <itkIdentityTransform.h>
#include <itkResampleImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkFileTools.h>

#include <itkNearestNeighborInterpolateImageFunction.h>
#include <itkLinearInterpolateImageFunction.h>
#include <itkBSplineInterpolateImageFunction.h>

#include <itkChangeLabelsImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkImageFileWriter.h>
#include <itkComposeImageFilter.h>

#include <itkVectorImage.h>
// #include <itkImageAlgorithm.h>
#include <itkIndexOfMaxImageFilter.h>
// #include <itkSmartPointer.h>

#include <dvImageToSet.h>

namespace dv
{

template<unsigned int Dimension, typename TPixel>
void
ResampleVolume(const std::string &IImage,
               const std::string &OImage,
               const double &spacing,
               const unsigned int &interpolator)
{
  using ImageType = itk::Image< TPixel, Dimension >;
  using VectorImageType = itk::VectorImage<double, Dimension>;
  using ReaderType = itk::ImageFileReader< ImageType >;
  using WriterType = itk::ImageFileWriter< ImageType >;
  using TransformType = itk::IdentityTransform< double, Dimension >;
  
  using ResampleType = itk::ResampleImageFilter< ImageType, ImageType >;
  using ChangeLabelsFilterType= itk::ChangeLabelsImageFilter< ImageType >;
  
  using ImageTypeML = itk::Image< double, Dimension >;
  using WriterTypeML = itk::ImageFileWriter< ImageTypeML >;
  using ResampleTypeML = itk::ResampleImageFilter< ImageType, ImageTypeML >;
  using ImageToVectorImageFilterType = itk::ComposeImageFilter<ImageTypeML>;
  using MaxFilterType = itk::IndexOfMaxImageFilter< VectorImageType,  ImageType >;
  // using MaxFilterType = itk::IndexOfMaxImageFilter< VectorImageType,  ImageTypeML >;

  using NNInterpolateType = itk::NearestNeighborInterpolateImageFunction< ImageType >;
  using LNInterpolateType = itk::LinearInterpolateImageFunction< ImageType >;
  using BSInterpolateType = itk::BSplineInterpolateImageFunction< ImageType >;
    
  const auto reader = ReaderType::New();
  const auto writer = WriterType::New();

  // Read input image file
  reader->SetFileName( IImage );
  reader->Update();

  // Graft onto an ImageType
  const auto image = ImageType::New();
  image->Graft( reader->GetOutput() );

  // Create ImageType for resampled output
  typename ImageType::SpacingType outputSpacing;
  outputSpacing.Fill( spacing );
  auto inputSize = image->GetLargestPossibleRegion().GetSize();
  auto inputSpacing = image->GetSpacing();
  typename ImageType::SizeType outputSize;
  for (unsigned int i = 0; i < Dimension; ++i)
    {
    outputSize[i] = std::floor(double(inputSize[i]) * inputSpacing[i] / outputSpacing[i]);
    }

  const auto resample = ResampleType::New();
  resample->SetTransform( TransformType::New() );
  resample->SetOutputStartIndex( image->GetLargestPossibleRegion().GetIndex() );
  resample->SetOutputOrigin( image->GetOrigin() );
  resample->SetOutputDirection( image->GetDirection() );
  resample->SetOutputSpacing( outputSpacing );
  resample->SetSize( outputSize );
  resample->SetInput( image );

  

  switch ( interpolator )
    {
    case 0:
      {
      const auto interp = NNInterpolateType::New();
      resample->SetInterpolator( interp );

        writer->SetInput( resample->GetOutput() );
        writer->SetFileName( OImage );
        writer->Update();

      break;
      }
    case 1:
      {
      const auto interp = LNInterpolateType::New();
      resample->SetInterpolator( interp );

        writer->SetInput( resample->GetOutput() );
        writer->SetFileName( OImage );
        writer->Update();

      break;
      }
    case 2:
      {
      const auto resampleML = ResampleTypeML::New();
      const auto writerML = WriterTypeML::New();

      resampleML->SetTransform( TransformType::New() );
      resampleML->SetOutputStartIndex( image->GetLargestPossibleRegion().GetIndex() );
      resampleML->SetOutputOrigin( image->GetOrigin() );
      resampleML->SetOutputDirection( image->GetDirection() );
      resampleML->SetOutputSpacing( outputSpacing );
      resampleML->SetSize( outputSize );

      // linearly interpolate classes separately
      const auto interp = LNInterpolateType::New();
      const auto imageToVectorImageFilter = ImageToVectorImageFilterType::New();
      
      // output label image name
      std::string cur_label_img = "temp.nii.gz";

      const auto labels_set = dv::ImageToSet<Dimension, TPixel>( image );
      std::map<TPixel, TPixel> label_map;
      for (const auto &x : labels_set) { int s = (int) x; label_map[s] = 0; }
      int counter = 0;;
      for (const auto &x : labels_set)
      {
        int s = (int) x;
        std::cout << std::to_string(s) << std::endl;
        // Ignore background.
        if (s == 0) continue;

        label_map[s] = 1;
        
        const auto filter = ChangeLabelsFilterType::New();
        filter->SetLabelMap( label_map );
        filter->SetInput( reader->GetOutput() );

        resampleML->SetInput( filter->GetOutput() );
        resampleML->SetInterpolator( interp );

        // writerML->SetInput( resampleML->GetOutput() );
        // writerML->SetFileName( "first_label.nii.gz" );
        // writerML->Update();
        

        // Add output of resample to composite filter
        resampleML->Update();
        imageToVectorImageFilter->SetInput(s - 1, resampleML->GetOutput());
        imageToVectorImageFilter->Update();
        
        label_map[s] = 0;
        counter++;
        if (counter == 2) break;
      }
      // imageToVectorImageFilter->Update();

      // const auto resampleML1 = ResampleTypeML::New();
      // const auto resampleML2 = ResampleTypeML::New();
      // std::map<TPixel, TPixel> label_map1;
      // for (const auto &x : labels_set) { int s = (int) x; label_map1[s] = 0; }
      // std::map<TPixel, TPixel> label_map2;
      // for (const auto &x : labels_set) { int s = (int) x; label_map2[s] = 0; }
      // label_map1[1] = 1;
      // label_map2[2] = 1;
      // const auto filter1 = ChangeLabelsFilterType::New();
      // const auto filter2 = ChangeLabelsFilterType::New();
      // filter1->SetLabelMap( label_map1 );
      // filter1->SetInput( reader->GetOutput() );
      // filter2->SetLabelMap( label_map2 );
      // filter2->SetInput( reader->GetOutput() );

      // resampleML1->SetTransform( TransformType::New() );
      // resampleML1->SetOutputStartIndex( image->GetLargestPossibleRegion().GetIndex() );
      // resampleML1->SetOutputOrigin( image->GetOrigin() );
      // resampleML1->SetOutputDirection( image->GetDirection() );
      // resampleML1->SetOutputSpacing( outputSpacing );
      // resampleML1->SetSize( outputSize );
      // resampleML1->SetInput( filter1->GetOutput() );
      // resampleML1->SetInterpolator( interp );

      // writerML->SetInput( resampleML1->GetOutput() );
      // writerML->SetFileName( "resampleML1.nii.gz" );
      // writerML->Update();

      // resampleML2->SetTransform( TransformType::New() );
      // resampleML2->SetOutputStartIndex( image->GetLargestPossibleRegion().GetIndex() );
      // resampleML2->SetOutputOrigin( image->GetOrigin() );
      // resampleML2->SetOutputDirection( image->GetDirection() );
      // resampleML2->SetOutputSpacing( outputSpacing );
      // resampleML2->SetSize( outputSize );
      // resampleML2->SetInput( filter2->GetOutput() );
      // resampleML2->SetInterpolator( interp );

      // writerML->SetInput( resampleML2->GetOutput() );
      // writerML->SetFileName( "resampleML2.nii.gz" );
      // writerML->Update();

      // imageToVectorImageFilter->SetInput(0, resampleML1->GetOutput());
      // imageToVectorImageFilter->SetInput(1, resampleML2->GetOutput());
      // imageToVectorImageFilter->Update();


      // get the max of each vector in VectorImage output from imageToVectorImageFilter
      const auto maxFilter = MaxFilterType::New();
      maxFilter->SetInput(imageToVectorImageFilter->GetOutput());
      maxFilter->Update();

      // writerML->SetInput( maxFilter->GetOutput() );
      // writerML->SetFileName( cur_label_img );
      // writerML->Update();

      writer->SetInput( maxFilter->GetOutput() );
      writer->SetFileName( cur_label_img );
      writer->Update();
      break;
      }
      default:
      {
      const auto interp = BSInterpolateType::New();
      interp->SetSplineOrder( interpolator );
      resample->SetInterpolator( interp );
      break;
      }
    }

  // writer->SetInput( resample->GetOutput() );
  // writer->SetFileName( OImage );
  // writer->Update();

}

}
#endif



        
        // Extract Label:
        // const auto thresh = TThresh::New();
        // thresh->SetInput( image );
        // thresh->SetLowerThreshold( s );
        // thresh->SetUpperThreshold( s );
        // thresh->SetInsideValue( s );
        // thresh->SetOutsideValue( 0 );