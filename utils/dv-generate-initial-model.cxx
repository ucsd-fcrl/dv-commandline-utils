// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkDefaultDynamicMeshTraits.h>
#include <itkMesh.h>
#include <itkQuadEdgeMesh.h>
#include <itkMeshFileReader.h>
#include <itkMeshFileWriter.h>

// => Connected
// => Delete Unassigned
// => Noise
// => Decimate
// => Connected
// => Delete Unassigned
// => Refine
// => Loop

#include <itkAdditiveGaussianNoiseMeshFilter.h>
#include <itkQuadEdgeMeshDecimationCriteria.h>
#include <itkSquaredEdgeLengthDecimationQuadEdgeMeshFilter.h>
#include <itkConnectedRegionsMeshFilter.h>
#include <dvDeleteIsolatedPoints.h>
#include <dvRefineValenceThreeVertices.h>
#include <itkLoopTriangleCellSubdivisionQuadEdgeMeshFilter.h>
 
const unsigned int Dimension = 3;
using TCoordinate = float;
 
using TMeshTraits = itk::DefaultDynamicMeshTraits< TCoordinate >;
using TMesh       = itk::Mesh< TCoordinate, Dimension, TMeshTraits >;
using TReader     = itk::MeshFileReader< TMesh >;
using TWriter     = itk::MeshFileWriter< TMesh >;

using TQEMesh       = itk::QuadEdgeMesh< TCoordinate, Dimension >;
using TQEReader     = itk::MeshFileReader< TQEMesh >;
using TQEWriter     = itk::MeshFileWriter< TQEMesh >;

using TConnected = itk::ConnectedRegionsMeshFilter< TMesh, TMesh >;
using TNoise = itk::AdditiveGaussianNoiseMeshFilter< TMesh >;
using TCriterion  = itk::NumberOfFacesCriterion< TQEMesh >;
using TDecimation = itk::SquaredEdgeLengthDecimationQuadEdgeMeshFilter< TQEMesh,
                                                              TQEMesh,
                                                              TCriterion >;
using TLoop = itk::LoopTriangleCellSubdivisionQuadEdgeMeshFilter< TQEMesh, TQEMesh >;
 
int
main( int argc, char ** argv )
{
 
  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-mesh",       po::value<std::string>()->required(),  "Filename of the input mesh.")
    ("output-directory", po::value<std::string>()->required(),  "Filename of the output mesh directory.")
    ("count",            po::value<unsigned int>()->default_value(500), "Target number of cells in the decimated mesh.")
    ("sigma",            po::value<double>()->default_value(0.1), "Amount of gaussian noise to add to mesh vertices prior to decimation.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc)
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const std::string inputMeshName(vm["input-mesh"].as<std::string>());
  const std::string outputMeshDirectory(vm["output-directory"].as<std::string>());
  const unsigned int count = vm["count"].as<unsigned int>();
  const double sigma = vm["sigma"].as<double>();

  const auto File0 = outputMeshDirectory + "00-Largest-Connected-Component.obj";
  const auto File1 = outputMeshDirectory + "01-Gaussian-Noise.obj";
  const auto File2 = outputMeshDirectory + "02-Decimated.obj";
  const auto File3 = outputMeshDirectory + "03-Isolated.obj";
  const auto File4 = outputMeshDirectory + "04-Subdivided.obj";

    {
    std::cout << "Extracting connected components..." << std::flush;

    const auto reader = TReader::New();
    reader->SetFileName( inputMeshName );

    const auto connected = TConnected::New();
    connected->SetInput( reader->GetOutput() );
    connected->SetExtractionModeToLargestRegion();

    const auto writer = TWriter::New();
    writer->SetInput( connected->GetOutput() );
    writer->SetFileName( File0 );
    writer->Update();

    std::cout << "done." << std::endl;
    }

    {
    std::cout << "Adding mesh noise..." << std::flush;

    const auto reader = TReader::New();
    reader->SetFileName( File0 );

    const auto noise = TNoise::New();
    noise->SetInput( reader->GetOutput() );
    noise->SetSigma( sigma );

    const auto writer = TWriter::New();
    writer->SetInput( noise->GetOutput() );
    writer->SetFileName( File1 );
    writer->Update();

    std::cout << "done." << std::endl;
    }

    {
    std::cout << "Decimating mesh..." << std::flush;

    const auto reader = TQEReader::New();
    reader->SetFileName( File1 );

    const auto criterion = TCriterion::New();
    const auto decimate = TDecimation::New();
 
    criterion->SetNumberOfElements(  count );

    decimate->SetInput( reader->GetOutput() );
    decimate->SetCriterion( criterion );
    decimate->Update();
    decimate->GetOutput()->SqueezePointsIds();

    const auto writer = TQEWriter::New();
    writer->SetInput( decimate->GetOutput() );
    writer->SetFileName( File2 );
    writer->Update();

    std::cout << "done." << std::endl;
    }

    {
    std::cout << "Deleting isolated points..." << std::flush;

    const auto reader = TReader::New();
    reader->SetFileName( File2 );

    const auto mesh = TMesh::New();
    mesh->Graft( reader->GetOutput() );
    mesh->DisconnectPipeline();

    dv::DeleteIsolatedPoints<TMesh>( mesh );

    const auto writer = TWriter::New();
    writer->SetInput( mesh );
    writer->SetFileName( File3 );
    writer->Update();

    std::cout << "done." << std::endl;
    }

    {
    std::cout << "Subdividing mesh..." << std::flush;

    const auto reader = TQEReader::New();
    reader->SetFileName( File3 );

    const auto loop = TLoop::New();
    loop->SetInput( reader->GetOutput() );

    const auto writer = TQEWriter::New();
    writer->SetInput( loop->GetOutput() );
    writer->SetFileName( File4 );

    writer->Update();

    std::cout << "done." << std::endl;
    }

  return EXIT_SUCCESS;
 
}
