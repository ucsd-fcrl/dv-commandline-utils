#ifndef dv_Explode4D_h
#define dv_Explode4D_h

// Boost
#include <boost/filesystem.hpp>

// ITK
#include <itkExtractImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

// Custom
#include <dvProgress.h>

namespace dv {

template<unsigned int Dimension, typename TPixel>
void
Explode4D(const std::string& IImage,
          const unsigned int& DimToExplode,
          const boost::filesystem::path& ODir)
{
  using TImage3D = itk::Image<TPixel, Dimension>;
  using TImage4D = itk::Image<TPixel, Dimension + 1>;
  using TReader = itk::ImageFileReader<TImage4D>;
  using TExtract = itk::ExtractImageFilter<TImage4D, TImage3D>;
  using TWriter = itk::ImageFileWriter<TImage3D>;

  const auto reader = TReader::New();
  reader->SetFileName(IImage);
  reader->Update();

  const auto image = TImage4D::New();
  image->Graft( reader->GetOutput() );

  const auto N = image->GetLargestPossibleRegion().GetSize()[DimToExplode];

  std::cout << "Exploding into " << N << " frames." << std::endl;

  auto progress = Progress(N);

  for (size_t i = 0; i < N; ++i) {

    auto region = image->GetLargestPossibleRegion();
    region.GetModifiableSize()[DimToExplode] = 0;
    region.GetModifiableIndex()[DimToExplode] = i;

    const auto extract = TExtract::New();
    extract->SetInput(image);
    extract->SetExtractionRegion(region);
    extract->SetDirectionCollapseToGuess();

    const auto output_file_name = std::to_string(i) + ".nii.gz";
    const auto output_file_path = ODir / output_file_name;

    const auto writer = TWriter::New();
    writer->SetInput(extract->GetOutput());
    writer->SetFileName(output_file_path.string());
    writer->Update();

    progress.UnitCompleted();

  }

}

}

#endif
