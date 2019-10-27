#ifndef dv_SmoothMesh_h
#define dv_SmoothMesh_h

// ITK
#include <itkMeshFileReader.h>
#include <itkMeshFileWriter.h>
#include <itkQuadEdgeMesh.h>
#include <itkQuadEdgeMeshParamMatrixCoefficients.h>
#include <itkSmoothingQuadEdgeMeshFilter.h>

namespace dv {

template<unsigned int Dimension, typename MeshPixelType>
void
SmoothMesh(const std::string& IMesh,
           const std::string& OImage,
           const unsigned int iterations = 1)
{

  using TMesh = itk::QuadEdgeMesh<MeshPixelType, Dimension>;

  using TReader = itk::MeshFileReader<TMesh>;
  using TSmooth = itk::SmoothingQuadEdgeMeshFilter<TMesh>;
  //  using TCoefficients = itk::InverseEuclideanDistanceMatrixCoefficients<
  //  TMesh >; using TCoefficients = itk::ConformalMatrixCoefficients< TMesh >;
  using TCoefficients = itk::AuthalicMatrixCoefficients<TMesh>;
  //  using TCoefficients = itk::IntrinsicMatrixCoefficients< TMesh >;
  //  using TCoefficients = itk::HarmonicMatrixCoefficients< TMesh >;
  using TWriter = itk::MeshFileWriter<TMesh>;

  const auto reader = TReader::New();
  reader->SetFileName(IMesh);

  TCoefficients mats;

  const auto smooth = TSmooth::New();
  smooth->SetInput(reader->GetOutput());
  smooth->SetCoefficientsMethod(&mats);
  smooth->SetRelaxationFactor(0.5);
  smooth->SetNumberOfIterations(iterations);

  const auto writer = TWriter::New();
  writer->SetFileName(OImage);
  writer->SetInput(smooth->GetOutput());
  writer->Update();
}

}

#endif
