
// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// VTK
#include <vtkActor.h>
#include <vtkArrowSource.h>
#include <vtkCamera.h>
#include <vtkMath.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

int
main(int argc, char** argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()("help", "Print usage information.");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc) {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
  }

  po::notify(vm);

  const unsigned int WindowX = 512;
  const unsigned int WindowY = 512;
  double BackgroundColor[3] = { 1.0, 1.0, 1.0 };
  const unsigned int TipResolution = 50;
  const unsigned int ShaftResolution = 50;

  const unsigned int N = 20;

  const auto renderer = vtkSmartPointer<vtkRenderer>::New();

  const auto matrix = vtkSmartPointer<vtkMatrix4x4>::New();
  matrix->SetElement(0, 0, -0.49832996);
  matrix->SetElement(0, 1, -0.28754944);
  matrix->SetElement(0, 2, -0.81791355);

  matrix->SetElement(1, 0, -0.49780063);
  matrix->SetElement(1, 1, -0.67749299);
  matrix->SetElement(1, 2, 0.5414774);

  matrix->SetElement(2, 0, -0.70983222);
  matrix->SetElement(2, 1, 0.67699229);
  matrix->SetElement(2, 2, 0.19447276);
  //[[-0.49832996 -0.49780063 -0.70983222]
  // [-0.28754944 -0.67749299  0.67699229]
  // [-0.81791355  0.5414774   0.19447276]]

  const auto transform = vtkSmartPointer<vtkTransform>::New();
  transform->SetMatrix(matrix);
  std::cout << *transform << std::endl;

  for (unsigned int i = 0; i < N; ++i) {
    const auto arrow = vtkSmartPointer<vtkArrowSource>::New();
    arrow->SetTipResolution(TipResolution);
    arrow->SetShaftResolution(ShaftResolution);
    const auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(arrow->GetOutputPort());
    const auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetPosition(
      0.0, std::sin(i * 2 * M_PI / N), std::cos(i * 2 * M_PI / N));
    actor->GetProperty()->SetColor(0.0, 1.0, 1.0);
    actor->GetProperty()->SetOpacity(static_cast<double>(i) / N);
    actor->SetMapper(mapper);
    actor->SetUserMatrix(transform->GetMatrix());
    renderer->AddActor(actor);
  }

  renderer->SetBackground(BackgroundColor);
  const auto window = vtkSmartPointer<vtkRenderWindow>::New();
  window->AddRenderer(renderer);
  window->SetSize(WindowX, WindowY);
  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(window);
  interactor->Start();

  return EXIT_SUCCESS;
}
