#include <itkMesh.h>
#include <itkMeshFileReader.h>
#include <itkMeshFileWriter.h>
#include <itkSTLMeshIO.h>
#include <itkTranslationTransform.h>
#include <itkTransformMeshFilter.h>

const unsigned int Dimension = 3;
typedef float      TCoordinate;

typedef itk::Mesh< TCoordinate, Dimension > TMesh;
typedef itk::MeshFileReader< TMesh >        TReader;
typedef itk::MeshFileWriter< TMesh >        TWriter;
typedef itk::TranslationTransform< TCoordinate, Dimension > TTranslate;
typedef itk::TransformMeshFilter< TMesh, TMesh, TTranslate > TTransform;

int
main( int argc, char* argv[] )
{

  if ( 6 != argc )
    {
    std::cerr << "Usage: "<< std::endl;
    std::cerr << argv[0] << " <InputMesh> <OutputMesh> <X> <Y> <Z>" << std::endl;
    return EXIT_FAILURE;
    }

  const std::string inputFileName(argv[1]);
  const std::string outputFileName(argv[2]);

  TCoordinate T[3] = {0.0, 0.0, 0.0};
  T[0] = std::atof(argv[3]);
  T[1] = std::atof(argv[4]);
  T[2] = std::atof(argv[5]);

  const auto reader = TReader::New();
  reader->SetFileName( inputFileName );
  reader->SetMeshIO( itk::STLMeshIO::New() );

  TTranslate::OutputVectorType displacement;
  for (std::size_t i = 0; i < 3; ++i) displacement[i] = T[i];

  const auto translate = TTranslate::New();
  translate->Translate( displacement );

  const auto transform = TTransform::New();
  transform->SetInput( reader->GetOutput() );
  transform->SetTransform( translate );

  const auto writer = TWriter::New();
  writer->SetInput( transform->GetOutput() );
  writer->SetFileName( outputFileName );
  writer->SetMeshIO( itk::STLMeshIO::New() );

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

