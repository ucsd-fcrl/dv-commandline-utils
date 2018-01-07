
// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkMesh.h>
#include <itkSimplexMesh.h>
#include <itkMeshFileReader.h>
#include <itkTriangleMeshToSimplexMeshFilter.h>
#include <itkSimplexMeshVolumeCalculator.h>

// Typedefs
using TMesh = itk::Mesh< float, 3 >;
using TSimplex = itk::SimplexMesh< float, 3 >;
using TReader = itk::MeshFileReader< TMesh >;
using TConvert = itk::TriangleMeshToSimplexMeshFilter< TMesh, TSimplex >;
using TVolume = itk::SimplexMeshVolumeCalculator< TSimplex >;
 
int main(int argc, char **argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-mesh", po::value<std::string>()->required(), "Filename of input mesh.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  // Create a spherical mesh with known radius and resolution.
  const auto reader = TReader::New();
  reader->SetFileName( vm["input-mesh"].as<std::string>() );
  reader->Update();
 
  std::cout << "Points: " << reader->GetOutput()->GetNumberOfPoints()       << std::endl;
  std::cout << "Cells: " << reader->GetOutput()->GetNumberOfCells()       << std::endl;

  // Ensure that all cells of the mesh are triangles.
  for (TMesh::CellsContainerIterator it = reader->GetOutput()->GetCells()->Begin();
       it != reader->GetOutput()->GetCells()->End();
       ++it)
    {   
    TMesh::CellAutoPointer cell;
    reader->GetOutput()->GetCell(it->Index(), cell);
    if (3 != cell->GetNumberOfPoints())
      {   
      std::cerr << "ERROR: All cells must be trianglar." << std::endl;  
      return EXIT_FAILURE;
      }   
    }   
 
  // Convert the triangle mesh to a simplex mesh.
  TConvert::Pointer convert = TConvert::New();
  convert->SetInput( reader->GetOutput() );
  convert->Update();
 
  // Calculate the volume and area of the simplex mesh.
  TVolume::Pointer volume = TVolume::New();
  volume->SetSimplexMesh( convert->GetOutput() );
  volume->Compute();
 
  // Compare with the volume and area of an ideal sphere.
  std::cout << "Volume: "        << volume->GetVolume()     << std::endl;
  std::cout << "Surface Area: "  << volume->GetArea()       << std::endl;
 
  return EXIT_SUCCESS;
 
}
