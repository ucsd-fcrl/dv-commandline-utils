// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// VTK
#include <vtkCallbackCommand.h>
#include <vtkCamera.h>
#include <vtkImageData.h>
#include <vtkPNGWriter.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

// Custom
#include <dvCycle.h>
#include <dvNumberOfSequentialFiles.h>
#include <dvStringOperations.h>

#include <dvVisualizeMeshesFrame.h>
#include <dvGetListOfColors.h>
#include <dvVisualizeMeshesInteractorStyle.h>

int
main(int argc, char** argv)
{

  // Declare the supported options.

  po::options_description description(
    "s : Capture screenshots.\n"
    "p : Load prior camera state.\n"
    "e,q : Exit.\n"
    "Allowed options");
  description.add_options()
    ("help", "Print usage information."
    )
    ("segmentation-directories",
     po::value<std::vector<std::string>>()->multitoken(),
     "Directory containing segmentation images named *.nii.gz."
    )
    ("labels",
     po::value<std::vector<unsigned int>>()->multitoken(),
     "Space-separated list of lables to visualize, e.g.: 0 1 2 3"
    )
    ("mesh-directories",
     po::value<std::vector<std::string>>()->multitoken(),
     "Directory containing meshes named *.obj."
    )
    ("screenshot-directory",
     po::value<std::string>(),
     "Directory in which to save screenshots."
    )
    ("camera-state",
     po::value<std::string>(),
     "JSON file containing the saved camera state."
    )
    ("downsampling-factor",
     po::value<double>()->default_value(1.0),
     "Downsampling factor."
    )
    ("window-size",
     po::value<unsigned int>()->default_value(512),
     "Window size."
    )
    ("restore-capture-quit",
     "Restore camera state, capture screenshots, and quit."
    );

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc) {
    std::cout << description << '\n';

    return EXIT_SUCCESS;
  }

  po::notify(vm);

  std::vector<std::string> SegmentationDirectories;
  if (vm.count("segmentation-directories")) {
    SegmentationDirectories = vm["segmentation-directories"].as<std::vector<std::string>>();
  }
  for (auto &_sd : SegmentationDirectories) {
    _sd = dv::AppendCharacterIfAbsent(_sd, '/');
  }

  std::vector<unsigned int> labels;
  if (vm.count("labels")) {
    labels = vm["labels"].as<std::vector<unsigned int>>();
  }

  const double SampleRate = vm["downsampling-factor"].as<double>();
  const unsigned int WindowSize = vm["window-size"].as<unsigned int>();

  std::vector<std::string> MeshDirectories;
  if (vm.count("mesh-directories")) {
    MeshDirectories = vm["mesh-directories"].as<std::vector<std::string>>();
  }

  const bool screenshot_dir_exists = vm.count("screenshot-directory");
  const std::string screenshot_dir =
    screenshot_dir_exists ? dv::AppendCharacterIfAbsent(
                              vm["screenshot-directory"].as<std::string>(), '/')
                          : "";

  std::vector<size_t> NumberOfFrames;

  for (const auto &dir : SegmentationDirectories) {
    const auto n = dv::NumberOfSequentialFiles([dir](size_t n) { return dir + std::to_string(n) + ".nii.gz"; });
    NumberOfFrames.push_back(n);
  }

  for (const auto &dir : MeshDirectories) {
    const auto n = dv::NumberOfSequentialFiles([dir](size_t n) { return dir + std::to_string(n) + ".obj"; });
    NumberOfFrames.push_back(n);
  }

  if (NumberOfFrames.size() == 0) {
    std::cerr << "No segmentations or meshes provided...returning." << std::endl;
    return EXIT_FAILURE;
  }

  for (const auto &n : NumberOfFrames) {
    assert(n == NumberOfFrames[0]);
  }

  const bool camera_state_exists = vm.count("camera-state");
  const std::string camera_state =
    camera_state_exists ? vm["camera-state"].as<std::string>() : "";

  const auto renderer = vtkSmartPointer<vtkRenderer>::New();
  renderer->SetBackground(1.0, 1.0, 1.0);

  const auto window = vtkSmartPointer<vtkRenderWindow>::New();
  window->AddRenderer(renderer);
  window->SetSize(WindowSize, WindowSize);

  // Necessary in order to properly render opacity of objects
  window->SetAlphaBitPlanes(true);
  window->SetMultiSamples(0);
  renderer->SetUseDepthPeeling(true);
  renderer->SetMaximumNumberOfPeels(4);
  renderer->SetOcclusionRatio(0.0);

  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->EnableRenderOn();

  std::vector<dv::VisualizeMeshesFrame> Frames;
  for (size_t i = 0; i < NumberOfFrames[0]; ++i) {

    auto frame = dv::VisualizeMeshesFrame(renderer);

    for (const auto &dir : MeshDirectories) {
      const auto FileName = dir + std::to_string(i) + ".obj";
      frame.AddMeshView(FileName);
    }

    for (const auto &dir : SegmentationDirectories) {
      const auto FileName = dir + std::to_string(i) + ".nii.gz";
      frame.AddSegmentationView(FileName, labels, dv::GetListOfColors());
    }

  Frames.push_back(frame);

  }

  const auto style = vtkSmartPointer<dv::VisualizeMeshesInteractorStyle>::New();
  style->m_Frames = Frames;

  style->screenshot_dir_exists = screenshot_dir_exists;
  style->screenshot_dir = screenshot_dir;

  style->camera_state_exists = camera_state_exists;
  style->camera_state = camera_state;

  style->index = dv::Cycle<size_t>{ NumberOfFrames[0] };
  interactor->SetInteractorStyle(style);
  style->SetCurrentRenderer(renderer);
  style->RenderForCurrentFrame();

  interactor->SetRenderWindow(window);

  if (camera_state_exists)
    {
    style->RestoreCameraState();
    }

  if (vm.count("restore-capture-quit")) {
    window->SetOffScreenRendering(true);
    style->CaptureScreenshots(false);
    window->Finalize();
  } else {
    window->Render();
    interactor->Start();
  }

  return EXIT_SUCCESS;
}
