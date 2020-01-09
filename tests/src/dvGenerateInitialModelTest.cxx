// Custom
#include <dvGenerateInitialModel.h>
#include <dvQuickViewPolyData.h>

// VTK
#include <vtkPolyDataReader.h>
#include <vtkSmartPointer.h>

int main() {

  const std::string file_name = "/Users/davisvigneault/Desktop/SiSSR-data/input/seg-nii/0.nii.gz";
  const double sigma = 0.25;
  const unsigned int count = 400;

  dv::GenerateInitialModel(
    file_name, "out.vtk", count, sigma
  );

  const auto reader = vtkSmartPointer<vtkPolyDataReader>::New();
  reader->SetFileName( "out.vtk" );
  reader->Update();

  dv::QuickViewPolyData( reader->GetOutput() );

  return EXIT_SUCCESS;

}
