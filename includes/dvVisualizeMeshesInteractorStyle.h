#ifndef dv_VisualizeMeshesInteractorStyle_h
#define dv_VisualizeMeshesInteractorStyle_h

// std
#include <set>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

// VTK
#include <vtkSmartPointer.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkWindowToImageFilter.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkPNGWriter.h>

// RapidJSON
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

// Boost
#include <boost/filesystem.hpp>

// Custom
#include <dvCameraState.h>
#include <dvCycle.h>
#include <dvProgress.h>
#include <dvSegmentationView.h>
#include <dvMeshView.h>
#include <dvVisualizeMeshesFrame.h>

// Define interaction style
namespace dv {

class VisualizeMeshesInteractorStyle : public vtkInteractorStyleTrackballCamera
{

public:
  static VisualizeMeshesInteractorStyle* New();
  vtkTypeMacro(VisualizeMeshesInteractorStyle, vtkInteractorStyleTrackballCamera);

  void OnKeyPress() override {
    const std::string key = this->Interactor->GetKeySym();

    // Toggle All On
    if (this->ToggleAllOnKeys.find(key) != this->ToggleAllOnKeys.cend()) {
      this->ToggleAllOn();
    }
    // Increment
    if (this->IncrementKeys.find(key) != this->IncrementKeys.cend()) {
      this->Increment();
    }
    // Decrement
    else if (this->DecrementKeys.find(key) != this->DecrementKeys.cend()) {
      this->Decrement();
    }
    // Take Screenshots
    else if (this->ScreenshotKeys.find(key) != this->ScreenshotKeys.cend()) {
      this->CaptureScreenshots(false);
    }
    // Take Rotating Screenshots
    else if (this->ScreenshotRotateKeys.find(key) !=
             this->ScreenshotRotateKeys.cend()) {
      this->CaptureScreenshots(true, 10);
    }
    // Restore Camera State
    else if (this->RestoreCameraStateKeys.find(key) !=
             this->RestoreCameraStateKeys.cend()) {
      this->RestoreCameraState();
    }

    // Forward events
    vtkInteractorStyleTrackballCamera::OnKeyPress();
  }

  void ToggleAllOn() {

    this->all_on = !this->all_on;
    if (this->all_on) {

      for (size_t i = 0; i < this->m_Frames.size(); ++i) {
        const auto hue = 1.0 - (static_cast<double>(i) / this->m_Frames.size());
        this->m_Frames[i].TurnAllActorsOn();
        this->m_Frames[i].SetHue( (hue + 1.0) / 2.0 );
      }
      this->GetCurrentRenderer()->GetRenderWindow()->Render();
    } else {

      this->RenderForCurrentFrame();

    }

  }

  void RenderForCurrentFrame() {
    for (auto &f : this->m_Frames) {
      f.TurnAllActorsOff();
    }
    this->m_Frames.at(index.GetCurrent()).TurnAllActorsOn();
    this->GetCurrentRenderer()->GetRenderWindow()->Render();
  }

  void Increment() {
    this->index.Increment();
    this->RenderForCurrentFrame();
  }

  void Decrement()
  {
    this->index.Decrement();
    this->RenderForCurrentFrame();
  }


  void CaptureScreenshots(const bool& rotating, const int& num_rotations = 1)
  {
    if (!this->screenshot_dir_exists) {
      std::cerr << "No screenshot directory was set." << std::endl;
      return;
    }
    const std::string time = std::to_string(std::time(nullptr));
    const std::string folder = this->screenshot_dir + time;
    if (!boost::filesystem::create_directories(
          boost::filesystem::path(folder))) {
      std::cerr << "The directory " << folder << " could not be created."
                << std::endl;
      return;
    }
    std::cout << "Saving screenshots to " << folder << "..." << std::endl;

    this->camera.CaptureState(this->GetCurrentRenderer()->GetActiveCamera());

    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();

    camera.SerializeJSON(writer);

    writer.EndObject();

    std::string cameraStateFileName =
      this->screenshot_dir + time + "/camera-state.json";
    std::ofstream fileStream;
    fileStream.open(cameraStateFileName);
    fileStream << sb.GetString();
    fileStream.close();

    const auto total_iterations =
      (rotating ? this->index.GetRange() * num_rotations
                : this->index.GetRange());
    const auto azimuth = 360.0 / total_iterations;

    auto progress = dv::Progress( total_iterations );

    for (size_t it = 0; it < total_iterations; ++it) {

      if (rotating) {
        this->GetCurrentRenderer()->GetActiveCamera()->OrthogonalizeViewUp();
        this->GetCurrentRenderer()->GetActiveCamera()->Azimuth(azimuth);
      }

      const auto screenshot = vtkSmartPointer<vtkWindowToImageFilter>::New();
      screenshot->SetInput(this->GetCurrentRenderer()->GetRenderWindow());
      screenshot->SetInputBufferTypeToRGBA();
      screenshot->SetFixBoundary(true);
      screenshot->Update();

      const auto writer = vtkSmartPointer<vtkPNGWriter>::New();

      std::string path;

      if (rotating) {
        path = folder + "/" + std::to_string(it) + ".png";
      } else {
        path = folder + "/" + std::to_string(this->index.GetCurrent()) + ".png";
      }

      writer->SetFileName(path.c_str());
      writer->SetInputConnection(screenshot->GetOutputPort());
      writer->Write();

      progress.UnitCompleted();
      this->Increment();
    }
  }

  void RestoreCameraState() {

    if (!this->camera_state_exists) {
      std::cerr << "No camera state was provided." << std::endl;
      return;
    }

    if (!boost::filesystem::exists(this->camera_state)) {
      std::cerr << "A camera state was provided, but the file does not exist."
                << std::endl;
    }

    std::ifstream file_stream;
    file_stream.open(this->camera_state);
    std::stringstream buffer;
    buffer << file_stream.rdbuf();
    file_stream.close();

    rapidjson::Document d;
    d.Parse(buffer.str().c_str());

    this->camera.DeserializeJSON(d);
    this->camera.RestoreState(this->GetCurrentRenderer()->GetActiveCamera());
    this->GetCurrentRenderer()->GetRenderWindow()->Render();
    this->GetInteractor()->InvokeEvent(vtkCommand::MouseWheelForwardEvent,
                                       NULL);
    this->GetInteractor()->InvokeEvent(vtkCommand::MouseWheelBackwardEvent,
                                       NULL);
  }

  dv::Cycle<size_t> index{ 1 };

  std::vector<VisualizeMeshesFrame> m_Frames;

  bool screenshot_dir_exists;
  std::string screenshot_dir;

  bool camera_state_exists;
  std::string camera_state;

  bool all_on = false;

  std::set<std::string> ToggleAllOnKeys{ "a" };
  std::set<std::string> IncrementKeys{ "Down", "Right", "j", "l" };
  std::set<std::string> DecrementKeys{ "Up", "Left", "h", "k" };
  std::set<std::string> ScreenshotKeys{ "s" };
  std::set<std::string> ScreenshotRotateKeys{ "S" };
  std::set<std::string> RestoreCameraStateKeys{ "p" };

  dv::CameraState camera;

};
}
vtkStandardNewMacro(dv::VisualizeMeshesInteractorStyle);

#endif
