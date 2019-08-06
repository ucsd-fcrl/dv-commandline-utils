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

#include <itkDelaunayConformingQuadEdgeMeshFilter.h>
#include <itkAdditiveGaussianNoiseQuadEdgeMeshFilter.h>
#include <itkQuadEdgeMeshDecimationCriteria.h>
#include <itkSquaredEdgeLengthDecimationQuadEdgeMeshFilter.h>
#include <itkConnectedRegionsMeshFilter.h>
#include <dvDeleteIsolatedPoints.h>
#include <dvSqueezePointsIds.h>
#include <dvRefineValenceThreeVertices.h>
#include <itkLoopTriangleCellSubdivisionQuadEdgeMeshFilter.h>
#include <dvMeshToQuadEdgeMesh.h>
 
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
using TNoise = itk::AdditiveGaussianNoiseQuadEdgeMeshFilter< TQEMesh >;
using TCriterion  = itk::NumberOfFacesCriterion< TQEMesh >;
using TDecimation = itk::SquaredEdgeLengthDecimationQuadEdgeMeshFilter< TQEMesh,
                                                              TQEMesh,
                                                              TCriterion >;
using TLoop = itk::LoopTriangleCellSubdivisionQuadEdgeMeshFilter< TQEMesh, TQEMesh >;
using TDelaunay = itk::DelaunayConformingQuadEdgeMeshFilter< TQEMesh >;

int
main( int argc, char ** argv )
{
 
  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-mesh",       po::value<std::string>()->required(),  "Filename of the input mesh.")
    ("output-mesh", po::value<std::string>()->required(),  "Filename of the output mesh.")
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
  const std::string outputMeshName(vm["output-mesh"].as<std::string>());
  const unsigned int count = vm["count"].as<unsigned int>();
  const double sigma = vm["sigma"].as<double>();

  const auto reader = TReader::New();
  reader->SetFileName( inputMeshName );

  const auto connected = TConnected::New();
  connected->SetInput( reader->GetOutput() );
  connected->SetExtractionModeToLargestRegion();
  connected->Update();

  const auto mesh = TMesh::New();
  mesh->Graft( connected->GetOutput() );
  mesh->DisconnectPipeline();

  dv::DeleteIsolatedPoints<TMesh>( mesh );
  dv::SqueezePointsIds<TMesh>( mesh );

  const auto qemesh = TQEMesh::New();
  dv::MeshToQuadEdgeMesh< TMesh, TQEMesh >( mesh, qemesh );

  const auto noise = TNoise::New();
  noise->SetInput( qemesh );
  noise->SetSigma( sigma );

  const auto criterion = TCriterion::New();
  const auto decimate = TDecimation::New();

  criterion->SetNumberOfElements(  count );

  decimate->SetInput( noise->GetOutput() );
  decimate->SetCriterion( criterion );

  const auto delaunay = TDelaunay::New();
  delaunay->SetInput( decimate->GetOutput() );

  const auto loop = TLoop::New();
  loop->SetInput( delaunay->GetOutput() );

  const auto writer = TQEWriter::New();
  writer->SetInput( loop->GetOutput() );
  writer->SetFileName( outputMeshName );
  writer->Update();

  return EXIT_SUCCESS;
 
}
