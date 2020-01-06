// ITK
#include <itkQuadEdgeMesh.h>
#include <itkMeshFileReader.h>

// Custom
#include <dvAssignCellDataByOctant.h>
#include <dvRefineValenceThreeVertices.h>
#include <dvITKTriangleMeshToVTKPolyData.h>
#include <dvQuickViewSideBySidePolyData.h>

const unsigned int Dimension = 3;
using TCoordinate = float;
using TMesh = itk::QuadEdgeMesh< TCoordinate, Dimension >;
using TReader = itk::MeshFileReader< TMesh >;

int main(int argc, char** argv) {

  if (argc < 2) {
    std::cerr << "Please supply test data directory." << std::endl;
  }

  const std::string test_data_dir(argv[1]);
  const std::string mesh_file = test_data_dir + "valence-three.obj";

  const auto reader = TReader::New();
  reader->SetFileName( mesh_file );
  reader->Update();

  const auto mesh = TMesh::New();
  mesh->Graft( reader->GetOutput() );

  dv::AssignCellDataByOctant< TMesh >( mesh );

  const auto i_polydata = dv::ITKTriangleMeshToVTKPolyData<TMesh>( mesh );

  dv::RefineValenceThreeVertices< TMesh >( mesh );

  std::cout << mesh->GetNumberOfCells() << std::endl;
  std::cout << mesh->GetCellData()->Size() << std::endl;

//  for (auto it = mesh->GetCells()->Begin(); it != mesh->GetCells()->End(); ++it) {
//    
//  }

  const auto o_polydata = dv::ITKTriangleMeshToVTKPolyData<TMesh>( mesh );

  dv::QuickViewSideBySidePolyData(i_polydata, o_polydata);

  return EXIT_SUCCESS;

}
