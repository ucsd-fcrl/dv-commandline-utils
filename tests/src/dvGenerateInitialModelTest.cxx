// Custom
#include <dvEnforceBoundaryBetweenLabels.h>
#include <dvExtractConnectedComponents.h>
#include <dvGenerateInitialModel.h>
#include <dvQuickViewPolyData.h>

// VTK
#include <vtkPolyDataReader.h>
#include <vtkSmartPointer.h>

int main() {

  const std::string file_name = "/home/davis/Dropbox/datasets/valve-plane-detection/derived_dir/00-spacing-2.0-multiplier-4/predictions/ucsd_toshiba/005/seg-nii-sm/0.nii.gz";
  const double sigma = 0.5;
  const unsigned int count = 1000;
  const unsigned int radius = 10;

  dv::GenerateInitialModel(
    file_name, "out.vtk", count, sigma, radius
  );

  const auto reader = vtkSmartPointer<vtkPolyDataReader>::New();
  reader->SetFileName( "out.vtk" );
  reader->Update();

  dv::QuickViewPolyData( reader->GetOutput() );

  return EXIT_SUCCESS;

}
