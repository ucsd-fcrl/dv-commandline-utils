
// ITK
#include <itkImageFileReader.h>
#include <itkExtractImageFilter.h>
#include <itkImageFileWriter.h>

namespace dv
{

template<unsigned int Dimension, typename TPixel>
void Collapse4D(const std::string &IImage,
                const unsigned int &DimToCollapse,
                const std::string &OImage)
{
  using TImage3D = itk::Image< TPixel, Dimension >;
  using TImage4D = itk::Image< TPixel, Dimension + 1 >;
  using TReader  = itk::ImageFileReader< TImage4D >;
  using TExtract = itk::ExtractImageFilter< TImage4D, TImage3D >;
  using TWriter  =itk::ImageFileWriter< TImage3D >;

  const auto reader = TReader::New();
  reader->SetFileName( IImage );
  reader->Update();

  auto region = reader->GetOutput()->GetLargestPossibleRegion();

  region.GetModifiableSize()[DimToCollapse] = 0;

  const auto extract = TExtract::New();
  extract->SetInput( reader->GetOutput() );
  extract->SetExtractionRegion( region );
  extract->SetDirectionCollapseToSubmatrix();

  const auto writer = TWriter::New();
  writer->SetInput( extract->GetOutput() );
  writer->SetFileName( OImage );
  writer->Update();
}

}

