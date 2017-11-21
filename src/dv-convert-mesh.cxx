// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkMesh.h>
#include <itkMeshFileReader.h>
#include <itkMeshFileWriter.h>
#include <itkSTLMeshIO.h>

const unsigned int Dimension = 3;
typedef float      TCoordinate;

typedef itk::Mesh< TCoordinate, Dimension > TMesh;
typedef itk::MeshFileReader< TMesh >        TReader;
typedef itk::MeshFileWriter< TMesh >        TWriter;

int main( int argc, char* argv[] )
{

  if ( 3 != argc )
    {
    std::cerr << "Usage:\n"<< std::endl;
    std::cerr << argv[0] << " <InputMesh> <OutputMesh>" << std::endl;
    return EXIT_FAILURE;
    }

  const std::string inputFileName(argv[1]);
  const std::string outputFileName(argv[2]);

  //
  // Reader
  //

  const auto reader = TReader::New();
  reader->SetFileName( inputFileName );

  const auto i_ext = inputFileName.substr(inputFileName.size() - 3, 3);
  if (i_ext == "stl" || i_ext == "STL")
    {
    reader->SetMeshIO( itk::STLMeshIO::New() );
    }

  //
  // Writer
  //

  const auto writer = TWriter::New();
  writer->SetInput( reader->GetOutput() );
  writer->SetFileName( outputFileName );

  const auto o_ext = outputFileName.substr(outputFileName.size() - 3, 3);
  if (o_ext == "stl" || o_ext == "STL")
    {
    writer->SetMeshIO( itk::STLMeshIO::New() );
    }

  try
    {
    writer->Update();
    }
  catch ( itk::ExceptionObject & err )
    {
    std::cerr << "There was a problem writing the file." << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;

}

