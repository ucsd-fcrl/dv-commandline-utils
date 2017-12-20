#ifndef dv_SegmentationError_h
#define dv_SegmentationError_h

// ITK
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkLabelOverlapMeasuresImageFilter.h>

namespace dv
{

template<unsigned int Dimension, typename TPixel>
void
SegmentationError(const std::string &IImage,
                  const std::string &OImage)
{
  using TImage = itk::Image< TPixel, Dimension >;
  using TReader = itk::ImageFileReader< TImage >;
  using TError = itk::LabelOverlapMeasuresImageFilter< TImage >;

  // Source
  const auto sourceReader = TReader::New();
  sourceReader->SetFileName( IImage );

  // Target
  const auto targetReader = TReader::New();
  targetReader->SetFileName( OImage );

  // Eror
  const auto error = TError::New();
  error->SetSourceImage( sourceReader->GetOutput() );
  error->SetTargetImage( targetReader->GetOutput() );
  error->Update();

  std::cout << "False Negative Error: " << error->GetFalseNegativeError( 1 ) << std::endl;
  std::cout << "False Positive Error: " << error->GetFalsePositiveError( 1 ) << std::endl;
  std::cout << "Target Overlap: "       << error->GetTargetOverlap( 1 )      << std::endl;
  std::cout << "Volume Similarity: "    << error->GetVolumeSimilarity( 1 )   << std::endl;
  std::cout << "UnionOverlap: "         << error->GetUnionOverlap( 1 )       << std::endl;
  std::cout << "Dice: "                 << error->GetDiceCoefficient( 1 ) << std::endl;
  std::cout << "Jaccard: "              << error->GetJaccardCoefficient( 1 ) << std::endl;

}

}

#endif
