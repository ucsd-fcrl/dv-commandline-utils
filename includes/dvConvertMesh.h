#ifndef dv_ConvertMesh_h
#define dv_ConvertMesh_h

// ITK
#include <itkMesh.h>
#include <itkMeshFileReader.h>
#include <itkMeshFileWriter.h>

namespace dv {

template<unsigned int Dimension, typename TCoordinate>
void
ConvertMesh(const std::string& IMesh, const std::string& OMesh)
{
  using TMesh = itk::Mesh<TCoordinate, Dimension>;
  using TReader = itk::MeshFileReader<TMesh>;
  using TWriter = itk::MeshFileWriter<TMesh>;

  //
  // Reader
  //

  const auto reader = TReader::New();
  reader->SetFileName(IMesh);

  //
  // Writer
  //

  const auto writer = TWriter::New();
  writer->SetInput(reader->GetOutput());
  writer->SetFileName(OMesh);
  writer->Update();
}

}

#endif
