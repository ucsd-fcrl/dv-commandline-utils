
// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// VTK
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkFillHolesFilter.h>
#include <vtkPolyDataNormals.h>

// Custom
#include <dvReadPolyData.h>
#include <dvWritePolyData.h>

int main(int argc, char *argv[])
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-mesh", po::value<std::string>()->required(), "Filename of the input mesh.")
    ("output-mesh", po::value<std::string>()->required(), "Filename of the output image.");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc) {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
  }

  po::notify(vm);

  const std::string inputFileName(vm["input-mesh"].as<std::string>());
  const std::string outputFileName(vm["output-mesh"].as<std::string>());

  vtkSmartPointer<vtkPolyData> input = dv::ReadPolyData(inputFileName);

  const auto fillHolesFilter = vtkSmartPointer<vtkFillHolesFilter>::New();
  fillHolesFilter->SetInputData(input);
  fillHolesFilter->SetHoleSize(100000.0);
  fillHolesFilter->Update();

  // Make the triangle winding order consistent
  const auto normals = vtkSmartPointer<vtkPolyDataNormals>::New();
  normals->SetInputData(fillHolesFilter->GetOutput());
  normals->ConsistencyOn();
  normals->SplittingOff();
  normals->Update();

  dv::WritePolyData(normals->GetOutput(), outputFileName);

  return EXIT_SUCCESS;
}

