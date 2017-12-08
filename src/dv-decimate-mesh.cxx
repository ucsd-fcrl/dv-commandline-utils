// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkQuadEdgeMesh.h>
#include <itkQuadEdgeMeshDecimationCriteria.h>
#include <itkSquaredEdgeLengthDecimationQuadEdgeMeshFilter.h>
#include <itkMeshFileReader.h>
#include <itkMeshFileWriter.h>
 
const unsigned int Dimension = 3;
using TCoordinate = float;
 
using TMesh       = itk::QuadEdgeMesh< TCoordinate, Dimension >;
using TReader     = itk::MeshFileReader< TMesh >;
using TWriter     = itk::MeshFileWriter< TMesh >;
using TCriterion  = itk::NumberOfFacesCriterion< TMesh >;
using TDecimation = itk::SquaredEdgeLengthDecimationQuadEdgeMeshFilter< TMesh,
                                                                        TMesh,
                                                                        TCriterion >;
 
int
main( int argc, char ** argv )
{
 
  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-mesh",  po::value<std::string>()->required(),    "Filename of the input mesh.")
    ("output-mesh", po::value<std::string>()->required(),    "Filename of the output image.")
    ("count",       po::value<unsigned int>()->required(),   "Target number of cells in output.")
    ("verbose",     po::value<bool>()->default_value(false), "Verbosity.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const std::string inputMeshName(vm["input-mesh"].as<std::string>());
  const std::string outputMeshName(vm["output-mesh"].as<std::string>());
  const unsigned int count = vm["count"].as<unsigned int>();
  const bool verbose = vm["verbose"].as<bool>();

  const auto criterion = TCriterion::New();
  const auto decimate = TDecimation::New();
 
  criterion->SetTopologicalChange( false );
  criterion->SetNumberOfElements(  count );

  const auto reader = TReader::New();
  reader->SetFileName( inputMeshName );
 
  decimate->SetInput( reader->GetOutput() );
  decimate->SetCriterion( criterion );

  if (verbose)
    {
    decimate->Update();

    std::cout << "Input mesh:\n"
              << "\tNumber of points: "  << reader->GetOutput()->GetNumberOfPoints()
              << "\n\tNumber of cells: " << reader->GetOutput()->GetNumberOfCells()
              << std::endl;
 
    std::cout << "Decimated mesh:\n"
              << "\tNumber of points: "  << decimate->GetOutput()->GetNumberOfPoints()
              << "\n\tNumber of cells: " << decimate->GetOutput()->GetNumberOfCells()
              << std::endl;
    }
 
  const auto writer = TWriter::New();
 
  writer->SetFileName( outputMeshName );
  writer->SetInput( decimate->GetOutput() );
  writer->Update();
 
  return EXIT_SUCCESS;
 
}
