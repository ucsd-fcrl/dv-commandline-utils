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

namespace po = boost::program_options;

int main(int argc, char** argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-mesh",      po::value<std::string>()->required(), "Filename of the input mesh.")
    ("reference-image", po::value<std::string>()->required(), "Filename of the reference image.")
    ("output-image",    po::value<std::string>()->required(), "Filename of the output image.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

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
    return EXIT_FAILURE;
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
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
