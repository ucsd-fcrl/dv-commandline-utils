// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// VTK
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkNIFTIImageReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkDiscreteMarchingCubes.h>
#include <vtkProperty.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCallbackCommand.h>
#include <vtkExtractVOI.h>

// Define interaction style
namespace dv
{
class KeyPressInteractorStyle : public vtkInteractorStyleTrackballCamera
{
  public:
    static KeyPressInteractorStyle* New();
    vtkTypeMacro(KeyPressInteractorStyle, vtkInteractorStyleTrackballCamera);
 
    void OnKeyPress() override
      {
      vtkRenderWindowInteractor *rwi = this->Interactor;
      const std::string key = rwi->GetKeySym();
 
      // Handle an arrow key
      if (this->IncrementKeys.find(key) != this->IncrementKeys.cend())
        {
        this->index += 1;
        this->UpdateReader();
        }
 
      // Handle an arrow key
      if (this->DecrementKeys.find(key) != this->DecrementKeys.cend())
        {
        this->index -= 1;
        this->UpdateReader();
        }
 
      // Forward events
      vtkInteractorStyleTrackballCamera::OnKeyPress();
      }

  void UpdateReader()
    {
    std::cout << "Frame: " << this->index << std::endl;
    const auto fn = this->directory + std::to_string(this->index) + ".nii.gz";
    this->reader->SetFileName( fn.c_str() );
    this->reader->Update();
    for (const auto &c : cubes) c->Update();
    this->GetCurrentRenderer()->GetRenderWindow()->Render();
    }
  unsigned int index = 0;
  std::string directory;
  vtkSmartPointer<vtkNIFTIImageReader> reader;
  std::vector<vtkSmartPointer<vtkDiscreteMarchingCubes>> cubes;
  std::set<std::string> IncrementKeys{"Down", "Right", "j", "l"};
  std::set<std::string> DecrementKeys{"Up", "Left", "h", "k"};
};
}
vtkStandardNewMacro(dv::KeyPressInteractorStyle);

int
main( int argc, char ** argv )
{
 
  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-directory", po::value<std::string>()->required(), "Directory containing segmentation images named *.nii.gz.")
    ("labels", po::value<std::vector<unsigned int>>()->multitoken()->required(), "Directory containing segmentation images named *.nii.gz.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const std::string dn = vm["input-directory"].as<std::string>();
  const std::vector<unsigned int> labels = vm["labels"].as<std::vector<unsigned int>>();
  const double SampleRate = 4.0;

  const auto renderer = vtkSmartPointer<vtkRenderer>::New();

  unsigned int frameid = 0;

  const auto reader = vtkSmartPointer<vtkNIFTIImageReader>::New();
  reader->SetFileName( (dn + std::to_string(frameid) + ".nii.gz").c_str() );
  reader->Update();

  const auto voi = vtkSmartPointer<vtkExtractVOI>::New();
  voi->SetInputConnection( reader->GetOutputPort() );
  voi->SetSampleRate( SampleRate, SampleRate, SampleRate );
  voi->SetVOI( reader->GetOutput()->GetExtent() );

  renderer->SetBackground( 1.0, 1.0, 1.0 );
  const auto window = vtkSmartPointer<vtkRenderWindow>::New();
  window->AddRenderer( renderer );
  window->SetSize( 512, 512 );
  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->EnableRenderOn();

  const auto style = vtkSmartPointer<dv::KeyPressInteractorStyle>::New();
  style->reader = reader;
  style->directory = dn;
  interactor->SetInteractorStyle( style );
  style->SetCurrentRenderer( renderer );

  interactor->SetRenderWindow( window );
  std::vector<std::array<double, 3>> colors;
  colors.emplace_back(std::array<double, 3>{  0.0/255,    0.0/255,    0.0/255});
  colors.emplace_back(std::array<double, 3>{255.0/255,    0.0/255,    0.0/255});
  colors.emplace_back(std::array<double, 3>{  0.0/255,  255.0/255,    0.0/255});
  colors.emplace_back(std::array<double, 3>{  0.0/255,    0.0/255,  255.0/255});
  colors.emplace_back(std::array<double, 3>{255.0/255,  255.0/255,    0.0/255});
  colors.emplace_back(std::array<double, 3>{  0.0/255,  255.0/255,  255.0/255});
  colors.emplace_back(std::array<double, 3>{255.0/255,    0.0/255,  255.0/255});
  colors.emplace_back(std::array<double, 3>{255.0/255,  127.0/255,    0.0/255});
  colors.emplace_back(std::array<double, 3>{  0.0/255,  255.0/255,  127.0/255});
  colors.emplace_back(std::array<double, 3>{127.0/255,    0.0/255,  255.0/255});
  colors.emplace_back(std::array<double, 3>{127.0/255,  255.0/255,    0.0/255});
  colors.emplace_back(std::array<double, 3>{  0.0/255,  127.0/255,  255.0/255});
  colors.emplace_back(std::array<double, 3>{255.0/255,    0.0/255,  127.0/255});
  colors.emplace_back(std::array<double, 3>{  0.0/255,   27.0/255,  155.0/255});
  colors.emplace_back(std::array<double, 3>{155.0/255,    0.0/255,   27.0/255});
  colors.emplace_back(std::array<double, 3>{127.0/255,  127.0/255,  127.0/255});

  for (const auto &l : labels)
    {
    const auto cubes = vtkSmartPointer<vtkDiscreteMarchingCubes>::New();
    cubes->SetInputConnection( voi->GetOutputPort() );

    cubes->SetValue( 0, l );

    const auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection( cubes->GetOutputPort() );
    mapper->ScalarVisibilityOff();
    const auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper( mapper );
    actor->GetProperty()->SetColor( colors.at(l).data() );
    renderer->AddActor( actor );
    style->cubes.push_back(cubes);
    }


  window->Render();

  interactor->Start();

  return EXIT_SUCCESS;
 
}
