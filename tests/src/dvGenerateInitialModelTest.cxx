// ITK
#include <itkImage.h>
#include <itkQuadEdgeMesh.h>
#include <itkImageFileReader.h>

// Custom
#include <itkGenerateInitialModelImageToMeshFilter.h>
#include <dvITKTriangleMeshToVTKPolyData.h>
#include <dvQuickViewPolyData.h>

int main() {

  const std::string file_name = "/home/davis/Dropbox/datasets/valve-plane-detection/derived_dir/00-spacing-2.0-multiplier-4/predictions/ucsd_toshiba/005/seg-nii-sm/0.nii.gz";
  const double sigma = 0.5;
  const unsigned int count = 1000;
  const unsigned int radius = 10;

  using TPixel = unsigned char;
  const unsigned int Dimension = 3;
  using TCoordinate = float;
  using TMesh = itk::QuadEdgeMesh<TCoordinate, Dimension>;

  using TImage = itk::Image<TPixel, Dimension>;
  using TReader = itk::ImageFileReader<TImage>;
  using TModel = itk::GenerateInitialModelImageToMeshFilter<TImage,TMesh>;

  const auto reader = TReader::New();
  reader->SetFileName(file_name);

  const auto model = TModel::New();
  model->SetInput(reader->GetOutput());
  model->SetNumberOfCellsInDecimatedMesh(count);
  model->SetMeshNoiseSigma(sigma);
  model->SetLVClosingRadius(radius);
  model->Update();

  const auto poly_data = dv::ITKTriangleMeshToVTKPolyData< TMesh >( model->GetOutput() );

  dv::QuickViewPolyData( poly_data );

  return EXIT_SUCCESS;

}
