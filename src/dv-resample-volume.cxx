// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

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

namespace po = boost::program_options;

// Typedefs
const unsigned int Dimension = 3;

using PixelType = signed short;
using ImageType = itk::Image< PixelType, Dimension >;
using ReaderType = itk::ImageFileReader< ImageType >;
using WriterType = itk::ImageFileWriter< ImageType >;
using TransformType = itk::IdentityTransform< double, Dimension >;
using ResampleType = itk::ResampleImageFilter< ImageType, ImageType >;

using NNInterpolateType = itk::NearestNeighborInterpolateImageFunction< ImageType >;
using LNInterpolateType = itk::LinearInterpolateImageFunction< ImageType >;
using BSInterpolateType = itk::BSplineInterpolateImageFunction< ImageType >;

int
main( int argc, char* argv[] )
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-image", po::value<std::string>()->required(), "Filename of input image.")
    ("output-image", po::value<std::string>()->required(), "Filename of output image.")
    ("spacing", po::value<double>(), "Desired spacing.")
    ("interpolator", po::value<unsigned int>()->default_value(1), "Order of BSpline interpolation (0 to 5 allowed).")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const auto reader = ReaderType::New();
  const auto resample = ResampleType::New();
  const auto writer = WriterType::New();

  reader->SetFileName( vm["input-image"].as<std::string>() );
  reader->Update();

  const auto image = ImageType::New();
  image->Graft( reader->GetOutput() );

  ImageType::SpacingType outputSpacing;
  outputSpacing.Fill( vm["spacing"].as<double>() );
  auto inputSize = image->GetLargestPossibleRegion().GetSize();
  auto inputSpacing = image->GetSpacing();
  ImageType::SizeType outputSize;
  for (unsigned int i = 0; i < Dimension; ++i)
    {
    outputSize[i] = std::floor(double(inputSize[i]) * inputSpacing[i] / outputSpacing[i]);
    }

  resample->SetTransform( TransformType::New() );
  resample->SetOutputStartIndex( image->GetLargestPossibleRegion().GetIndex() );
  resample->SetOutputOrigin( image->GetOrigin() );
  resample->SetOutputDirection( image->GetDirection() );
  resample->SetOutputSpacing( outputSpacing );
  resample->SetSize( outputSize );
  resample->SetInput( image );

  switch (vm["interpolator"].as<unsigned int>())
    {
    case 0:
      {
      const auto interp = NNInterpolateType::New();
      resample->SetInterpolator( interp );
      break;
      }
    case 1:
      {
      const auto interp = LNInterpolateType::New();
      resample->SetInterpolator( interp );
      break;
      }
    default:
      {
      const auto interp = BSInterpolateType::New();
      interp->SetSplineOrder( vm["interpolator"].as<unsigned int>() );
      resample->SetInterpolator( interp );
      break;
      }
    }

  writer->SetInput( resample->GetOutput() );
  writer->SetFileName( vm["output-image"].as<std::string>() );
  writer->Update();

  return EXIT_SUCCESS;

}

