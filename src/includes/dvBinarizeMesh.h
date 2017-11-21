
// ITK
#include <itkMesh.h>
#include <itkMeshFileReader.h>
#include <itkSTLMeshIO.h>

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

#include <itkTriangleMeshToBinaryImageFilter.h>

namespace dv
{

template<unsigned int Dimension, typename MeshPixelType, typename ImagePixelType>
void
BinarizeMesh(po::variables_map vm)
{

  typedef itk::Mesh< MeshPixelType, Dimension > MeshType;

  typedef itk::MeshFileReader< MeshType >  MeshReaderType;

  typedef itk::Image< ImagePixelType, Dimension > InputImageType;
  typedef itk::ImageFileReader< InputImageType >  ImageReaderType;

  typedef unsigned char                             OutputPixelType;
  typedef itk::Image< OutputPixelType, Dimension >  OutputImageType;

  typedef itk::TriangleMeshToBinaryImageFilter< MeshType, OutputImageType > FilterType;

  typedef itk::ImageFileWriter< OutputImageType > WriterType;

  const auto meshReader = MeshReaderType::New();
  meshReader->SetFileName( vm["input-mesh"].as<std::string>() );

  const auto i_ext = vm["input-mesh"].as<std::string>().substr(vm["input-mesh"].as<std::string>().size() - 3, 3);
  if (i_ext == "stl" || i_ext == "STL")
    {
    meshReader->SetMeshIO( itk::STLMeshIO::New() );
    }

  const auto imageReader = ImageReaderType::New();
  imageReader->SetFileName( vm["reference-image"].as<std::string>() );

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
//    return EXIT_FAILURE;
    }

  const auto writer = WriterType::New();
  writer->SetFileName( vm["output-image"].as<std::string>() );
  writer->SetInput( filter->GetOutput() );

  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & error )
    {
    std::cerr << "Error: " << error << std::endl;
//    return EXIT_FAILURE;
    }

}

}

