// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkMesh.h>
#include <itkMeshFileReader.h>
#include <itkPointsLocator.h>
#include <itkMeshFileWriter.h>

// STD
#include <numeric>
 
const unsigned int Dimension = 3;
using TCoordinate = float;
 
using TMesh   = itk::Mesh< TCoordinate, Dimension >;
using TReader = itk::MeshFileReader< TMesh >;
using TTree   = itk::PointsLocator< TMesh::PointsContainer>;
 
int
main( int argc, char ** argv )
{
 
  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("fixed-mesh",  po::value<std::string>()->required(), "Filename of the input mesh.")
    ("moving-mesh", po::value<std::string>()->required(), "Filename of the input mesh.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const std::string fixedMeshName(vm["fixed-mesh"].as<std::string>());
  const std::string movingMeshName(vm["moving-mesh"].as<std::string>());

  const auto fixedReader = TReader::New();
  fixedReader->SetFileName( fixedMeshName );
  fixedReader->Update();

  const auto movingReader = TReader::New();
  movingReader->SetFileName( movingMeshName );
  movingReader->Update();

  const auto tree = TTree::New();
  tree->SetPoints( movingReader->GetOutput()->GetPoints() );
  tree->Initialize();

  std::vector<double> dist;

  for (auto p = fixedReader->GetOutput()->GetPoints()->Begin();
       p != fixedReader->GetOutput()->GetPoints()->End();
       ++p)
    {
    const auto id = tree->FindClosestPoint( p.Value() );
    const auto pt = tree->GetPoints()->ElementAt( id );
    dist.emplace_back( pt.EuclideanDistanceTo( p.Value() ) );
    }

  const auto min = *std::min_element( std::begin(dist), std::end(dist) );
  const auto max = *std::max_element( std::begin(dist), std::end(dist) );
  const auto mean = std::accumulate( std::begin(dist), std::end(dist), 0.0 ) / dist.size();
  const size_t n = dist.size() / 2;
  std::nth_element(std::begin(dist), std::begin(dist) + n, std::end(dist));
  const auto median = dist[n];

  std::cout << "Min: " << min << std::endl;
  std::cout << "Max: " << max << std::endl;
  std::cout << "Mean: " << mean << std::endl;
  std::cout << "Median: " << median << std::endl;

  return EXIT_SUCCESS;
 
}
