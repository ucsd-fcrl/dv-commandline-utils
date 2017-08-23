#include <itkImage.h>
#include <itkIdentityTransform.h>
#include <itkResampleImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkFileTools.h>

// Typedefs
const unsigned int Dimension = 3;

typedef signed short                                     PixelType;
typedef itk::Image< PixelType, Dimension >               ImageType;
typedef itk::ImageFileReader< ImageType >                ReaderType;
typedef itk::ImageFileWriter< ImageType >                WriterType;
typedef itk::IdentityTransform< double, Dimension >      TransformType;
typedef itk::ResampleImageFilter< ImageType, ImageType > ResampleType;

int
main( int argc, char* argv[] )
{

  // Deal with commandline arguments
  if (3 > argc)
    {
    std::cerr << "Usage:\n"
              << argv[0] << " <inputDir> <outputDir> [spacing] [ext]" << std::endl;
    return EXIT_FAILURE;
    }

  std::string inputDirectory = argv[1];
  std::string outputDirectory = argv[2];
  const double spacing = (argc > 3) ? atof(argv[3]) : 0.5;
  const std::string ext = (argc > 4) ? argv[4] : "vtk";

  itk::FileTools::CreateDirectory(outputDirectory.c_str());

  // Make sure that the directories have slashes at the end
  if ('/' != inputDirectory.back()) inputDirectory += '/';
  if ('/' != outputDirectory.back()) outputDirectory += '/';

  std::vector<std::string> FileNames;
  while (
    itksys::SystemTools::FileExists(
      inputDirectory + std::to_string(FileNames.size()) + "." + ext,true)
        ) FileNames.emplace_back(std::to_string(FileNames.size()) + "." + ext);

  itkAssertOrThrowMacro(FileNames.size() > 0, "At least one image must be supplied.");

  auto reader = ReaderType::New();
  auto resample = ResampleType::New();
  auto writer = WriterType::New();

  for (auto file : FileNames)
    {
    reader->SetFileName( inputDirectory + file );
    reader->Update();

    auto image = ImageType::New();
    image->Graft( reader->GetOutput() );

    ImageType::SpacingType outputSpacing;
    outputSpacing.Fill( spacing );
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

    writer->SetInput( resample->GetOutput() );
    writer->SetFileName( outputDirectory + file );
    writer->Update();

    }

  return EXIT_SUCCESS;

}

