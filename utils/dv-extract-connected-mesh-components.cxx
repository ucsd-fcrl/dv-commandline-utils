#include <dvDeleteIsolatedPoints.h>
// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkQuadEdgeMesh.h>
#include <itkMesh.h>
#include <itkMeshFileReader.h>
#include <itkMeshFileWriter.h>
#include <itkTranslationTransform.h>
#include <itkTransformMeshFilter.h>
#include <itksys/SystemTools.hxx>
#include <itkConnectedRegionsMeshFilter.h>
#include <itkSphereMeshSource.h>
#include <itkDefaultDynamicMeshTraits.h>

const unsigned int Dimension = 3;
using TCoordinate = float;

using TMeshTraits = itk::DefaultDynamicMeshTraits< TCoordinate >;
using TMesh      = itk::Mesh< TCoordinate, Dimension, TMeshTraits >;
//using TMesh      = itk::QuadEdgeMesh< TCoordinate, Dimension >;
using TReader    = itk::MeshFileReader< TMesh >;
using TWriter    = itk::MeshFileWriter< TMesh >;
using TTranslate = itk::TranslationTransform< TCoordinate, Dimension >;
using TTransform = itk::TransformMeshFilter< TMesh, TMesh, TTranslate >;
using TConnected = itk::ConnectedRegionsMeshFilter< TMesh, TMesh >;
using TSphere = itk::SphereMeshSource< TMesh >;


int
main( int argc, char* argv[] )
{


  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-mesh",  po::value<std::string>()->required(), "Filename of the input mesh.")
    ("output-mesh", po::value<std::string>()->required(), "Filename of the output image.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc)
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const std::string inputFileName(vm["input-mesh"].as<std::string>());
  const std::string outputFileName(vm["output-mesh"].as<std::string>());

  const auto reader = TReader::New();
  reader->SetFileName( inputFileName );

  const auto connect = TConnected::New();
  connect->SetInput( reader->GetOutput() );
  connect->SetExtractionModeToLargestRegion();
  connect->Update();

  const auto mesh = TMesh::New();
  mesh->Graft( connect->GetOutput() );
  mesh->DisconnectPipeline();

  dv::DeleteIsolatedPoints<TMesh>( mesh );

  const auto writer = TWriter::New();
  writer->SetInput( mesh );
  writer->SetFileName( outputFileName );
  writer->Update();

//  try
//    {
//    writer->Update();
//    }
//  catch ( itk::ExceptionObject & err )
//    {
//    std::cerr << "There was a problem writing the file." << std::endl;
//    std::cerr << err << std::endl;
//    return EXIT_FAILURE;
//    }

  return EXIT_SUCCESS;

}

