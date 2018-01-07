#include <itkMesh.h>
#include <itkSimplexMesh.h>
#include <itkMeshFileReader.h>
#include <itkTriangleMeshToSimplexMeshFilter.h>
#include <itkSimplexMeshVolumeCalculator.h>
 
typedef itk::Mesh< float, 3 >        TMesh;
typedef itk::SimplexMesh< float, 3 > TSimplex;
typedef itk::MeshFileReader< TMesh > TReader;
typedef itk::TriangleMeshToSimplexMeshFilter< TMesh, TSimplex > TConvert;
typedef itk::SimplexMeshVolumeCalculator< TSimplex >            TVolume;
 
int main(int argc, char **argv)
{

  // Create a spherical mesh with known radius and resolution.
  const auto reader = TReader::New();
  reader->SetFileName( argv[1] );
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
