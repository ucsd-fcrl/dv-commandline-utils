#ifndef dv_WindowImage_h
#define dv_WindowImage_h

// ITK includes
#include <itkImage.h>
#include <itkImageFileWriter.h>
#include <itkIntensityWindowingImageFilter.h>
#include <itkImageRegionIterator.h>
#include <itkNumericTraits.h>

namespace dv
{

  template<unsigned int Dimension, typename TPixel>
  void
  WindowImage(const std::string &IImage,
    const std::string &OImage,
    const itk::FixedArray<signed int, 2> &scale,
    const itk::FixedArray<signed int, 2> &o_range)
    {

      using TImage = itk::Image<TPixel, Dimension>;

      using TReader = itk::ImageFileReader<TImage>;
      const auto reader = TReader::New();
      reader->SetFileName( IImage );


      const auto i_ext = OImage.substr(OImage.size() - 3, 3);
      if (i_ext == "png")
      {
        using TImagePNG = itk::Image<unsigned short, Dimension>;
        using TFilterPNG   = itk::IntensityWindowingImageFilter<TImage,TImagePNG>;
        const auto filter = TFilterPNG::New();
        filter->SetInput( reader->GetOutput() );
        filter->SetWindowMinimum(scale[0]);
        filter->SetWindowMaximum(scale[1]);
        filter->SetOutputMinimum(itk::NumericTraits<unsigned short>::min());
        filter->SetOutputMaximum(itk::NumericTraits<unsigned short>::max());
        filter->Update();

        // Add a cast of the image to unsigned SHORT
        using TWriterPNG = itk::ImageFileWriter<TImagePNG>;
        const auto writer = TWriterPNG::New();
        writer->SetInput( filter->GetOutput() );
        writer->SetFileName( OImage );
        writer->Update();
      }
      else
      {
        using TFilter   = itk::IntensityWindowingImageFilter<TImage>;
        const auto filter = TFilter::New();
        filter->SetInput( reader->GetOutput() );
        filter->SetWindowMinimum(scale[0]);
        filter->SetWindowMaximum(scale[1]);
        filter->SetOutputMinimum(o_range[0]);
        filter->SetOutputMaximum(o_range[1]);
        filter->Update();

        using TWriter = itk::ImageFileWriter<TImage>;
        const auto writer = TWriter::New();
        writer->SetInput( filter->GetOutput() );
        writer->SetFileName( OImage );
        writer->Update();
      }


      return EXIT_SUCCESS;
    }
  }
  #endif
