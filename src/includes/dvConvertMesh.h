
// ITK
#include <itkMesh.h>
#include <itkMeshFileReader.h>
#include <itkMeshFileWriter.h>
#include <itkSTLMeshIO.h>

namespace dv
{

template<unsigned int Dimension, typename TCoordinate>
void
ConvertMesh(const std::string &IMesh,
            const std::string &OMesh)
{
  using TMesh   = itk::Mesh< TCoordinate, Dimension >;
  using TReader = itk::MeshFileReader< TMesh >;
  using TWriter = itk::MeshFileWriter< TMesh >;

  //
  // Reader
  //

  const auto reader = TReader::New();
  reader->SetFileName( IMesh );

  const auto i_ext = IMesh.substr(IMesh.size() - 3, 3);
  if (i_ext == "stl" || i_ext == "STL")
    {
    reader->SetMeshIO( itk::STLMeshIO::New() );
    }

  //
  // Writer
  //

  const auto writer = TWriter::New();
  writer->SetInput( reader->GetOutput() );
  writer->SetFileName( OMesh );

  const auto o_ext = OMesh.substr(OMesh.size() - 3, 3);
  if (o_ext == "stl" || o_ext == "STL")
    {
    writer->SetMeshIO( itk::STLMeshIO::New() );
    }

  writer->Update();

}

}
