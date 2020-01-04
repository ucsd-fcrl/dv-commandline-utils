// ITK
#include <itkImage.h>
#include <itkQuadEdgeMesh.h>
#include <itkCuberilleImageToMeshFilter.h>

// VTK
#include <vtkPolyData.h>

// Custom
#include <dvITKTriangleMeshToVTKPolyData.h>
#include <dvQuickViewPolyData.h>

using TPixel = unsigned char;
using TCoordinate = float;
const unsigned int Dimension = 3;
using TImage = itk::Image<TPixel, Dimension>;
//using TMesh = itk::QuadEdgeMesh<TCoordinate, Dimension>;
using TMesh = itk::Mesh<TCoordinate, Dimension>;
using TExtract = itk::CuberilleImageToMeshFilter< TImage, TMesh >;

int main() {

  const auto image = TImage::New();
  TImage::SizeType size;
  size.Fill( 5 );

  TImage::IndexType origin;
  origin.Fill( 0 );

  TImage::RegionType region(origin, size);

  image->SetRegions( region );
  image->Allocate();
  image->FillBuffer( 0 );
  image->SetPixel( {{1, 1, 1}}, 1 );
  image->SetPixel( {{2, 2, 2}}, 2 );
  image->SetPixel( {{3, 3, 2}}, 3 );

  const auto extract = TExtract::New();
  extract->SetInput( image );
  extract->GenerateTriangleFacesOn();
  extract->ProjectVerticesToIsoSurfaceOff();
  extract->SavePixelAsCellDataOn();
  extract->Update();

  // Visualize
  const auto polydata = dv::ITKTriangleMeshToVTKPolyData<TMesh>( extract->GetOutput() );

  dv::QuickViewPolyData( polydata );

  return EXIT_SUCCESS;

}
