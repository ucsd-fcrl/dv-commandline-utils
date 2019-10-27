// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkIterativeTriangleCellSubdivisionQuadEdgeMeshFilter.h>
#include <itkLoopTriangleCellSubdivisionQuadEdgeMeshFilter.h>
#include <itkMeshFileReader.h>
#include <itkMeshFileWriter.h>
#include <itkQuadEdgeMesh.h>

const unsigned int Dimension = 3;
using TCoordinate = float;

using TMesh = itk::QuadEdgeMesh<TCoordinate, Dimension>;
using TReader = itk::MeshFileReader<TMesh>;
using TLoop = itk::LoopTriangleCellSubdivisionQuadEdgeMeshFilter<TMesh, TMesh>;
using TSubdivide =
  itk::IterativeTriangleCellSubdivisionQuadEdgeMeshFilter<TMesh, TLoop>;
using TWriter = itk::MeshFileWriter<TMesh>;

int
main(int argc, char** argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()("help", "Print usage information.")(
    "input-mesh",
    po::value<std::string>()->required(),
    "Filename of the input mesh.")("output-mesh",
                                   po::value<std::string>()->required(),
                                   "Filename of the output image.")(
    "n", po::value<unsigned int>()->required(), "Number of subdivisions.");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc) {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
  }

  po::notify(vm);

  const std::string inputMeshName(vm["input-mesh"].as<std::string>());
  const std::string outputMeshName(vm["output-mesh"].as<std::string>());
  const unsigned int n = vm["n"].as<unsigned int>();

  const auto reader = TReader::New();
  reader->SetFileName(inputMeshName);

  const auto subdivide = TSubdivide::New();
  subdivide->SetInput(reader->GetOutput());
  subdivide->SetResolutionLevels(n);

  const auto writer = TWriter::New();
  writer->SetFileName(outputMeshName);
  writer->SetInput(subdivide->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
