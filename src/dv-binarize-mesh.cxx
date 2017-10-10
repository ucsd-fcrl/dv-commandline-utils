// Boost
#include <boost/program_options.hpp>

// ITK
#include <itkMesh.h>
#include <itkMeshFileReader.h>
#include <itkSTLMeshIO.h>

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

#include <itkTriangleMeshToBinaryImageFilter.h>

const unsigned int Dimension = 3;
typedef double MeshPixelType;

typedef itk::Mesh< MeshPixelType, Dimension > MeshType;

typedef itk::MeshFileReader< MeshType >  MeshReaderType;

typedef unsigned char                           InputPixelType;
typedef itk::Image< InputPixelType, Dimension > InputImageType;
typedef itk::ImageFileReader< InputImageType >  ImageReaderType;

typedef unsigned char                             OutputPixelType;
typedef itk::Image< OutputPixelType, Dimension >  OutputImageType;

typedef itk::TriangleMeshToBinaryImageFilter< MeshType, OutputImageType > FilterType;

typedef itk::ImageFileWriter< OutputImageType > WriterType;

int main(int argc, char** argv)
{

  if (argc != 4)
    {
    std::cerr << "Usage:\n"
              << argv[0]
              << " <InputImageName> <InputMeshName> <OutputImageName>"
              << std::endl;
    return EXIT_FAILURE;
    }

  const std::string inputImageName  = argv[1];
  const std::string inputMeshName   = argv[2];
  const std::string outputImageName = argv[3];

  const auto meshReader = MeshReaderType::New();
  meshReader->SetFileName( inputMeshName );

  const auto i_ext = inputMeshName.substr(inputMeshName.size() - 3, 3);
  if (i_ext == "stl" || i_ext == "STL")
    {
    meshReader->SetMeshIO( itk::STLMeshIO::New() );
    }

  const auto imageReader = ImageReaderType::New();
  imageReader->SetFileName( inputImageName );

  const auto filter = FilterType::New();
  filter->SetInput( meshReader->GetOutput() );
  filter->SetInfoImage( imageReader->GetOutput() );
  filter->SetInsideValue( 1 );

  try
    {
    filter->Update();
    }
  catch( itk::ExceptionObject & error )
    {
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
    }

  const auto writer = WriterType::New();
  writer->SetFileName( outputImageName );
  writer->SetInput( filter->GetOutput() );

  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & error )
    {
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
