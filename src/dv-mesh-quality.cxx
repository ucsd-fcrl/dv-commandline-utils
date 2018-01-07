
// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// VTK
#include <vtkOBJReader.h>
#include <vtkMeshQuality.h>
#include <vtkSmartPointer.h>
#include <vtkFieldData.h>
#include <vtkDoubleArray.h>
#include <vtkCellData.h>
 
const unsigned int Dimension = 3;
using TCoordinate = float;

using TReader = vtkSmartPointer<vtkOBJReader>;
using TQuality = vtkSmartPointer<vtkMeshQuality>;
 
int
main( int argc, char ** argv )
{
 
  std::map<int, std::string> quality_map;
  quality_map[VTK_QUALITY_AREA]                  = "TriangleArea";
  quality_map[VTK_QUALITY_RADIUS_RATIO]          = "TriangleRadiusRatio";
  quality_map[VTK_QUALITY_ASPECT_RATIO]          = "TriangleAspectRatio";
  quality_map[VTK_QUALITY_ASPECT_FROBENIUS]      = "TriangleAspectFrobenius";
  quality_map[VTK_QUALITY_EDGE_RATIO]            = "TriangleEdgeRatio";
  quality_map[VTK_QUALITY_MIN_ANGLE]             = "TriangleMinAngle";
  quality_map[VTK_QUALITY_MAX_ANGLE]             = "TriangleMaxAngle";
  quality_map[VTK_QUALITY_CONDITION]             = "TriangleCondition";
  quality_map[VTK_QUALITY_SCALED_JACOBIAN]       = "TriangleScaledJacobian";
  quality_map[VTK_QUALITY_RELATIVE_SIZE_SQUARED] = "TriangleRelativeSizeSquared";
  quality_map[VTK_QUALITY_SHAPE]                 = "TriangleShape";
  quality_map[VTK_QUALITY_SHAPE_AND_SIZE]        = "TriangleShapeAndSize";
  quality_map[VTK_QUALITY_DISTORTION]            = "TriangleDistortion";

  std::string metric_description = "Integer corresponding to the quality metric.\n";
  for (const auto& pair : quality_map)
    {
    metric_description += ('\t' + std::to_string(pair.first) + ' ' + pair.second + '\n');
    }

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-mesh", po::value<std::string>()->required(), "Filename of the input mesh.")
    ("metric", po::value<int>()->default_value(VTK_QUALITY_RADIUS_RATIO), metric_description.c_str())
    ("cellwise", po::value<bool>()->default_value(false), "Calculate cellwise quality.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const std::string IMesh(vm["input-mesh"].as<std::string>());
  const int metric = vm["metric"].as<int>();
  const bool cellwise = vm["cellwise"].as<bool>();

  const auto reader = TReader::New();
  reader->SetFileName( IMesh.c_str() );

  const auto quality = TQuality::New();
  quality->SetInputConnection( reader->GetOutputPort() );
  if (cellwise)
    {
    quality->SaveCellQualityOn();
    }
  else
    {
    quality->SaveCellQualityOff();
    }
  quality->SetTriangleQualityMeasure(metric);
  quality->Update();
  const auto fd = quality->GetOutput()->GetFieldData()->GetArray( "Mesh Triangle Quality" );
  const auto mn = fd->GetComponent( 0, 0 );
  const auto mu = fd->GetComponent( 0, 1 );
  const auto mx = fd->GetComponent( 0, 2 );
  const auto sd = fd->GetComponent( 0, 3 );

  std::cout << "Metric: " << quality_map[metric] << std::endl;
  std::cout << "Min: " << mn << std::endl;
  std::cout << "Average: " << mu << std::endl;
  std::cout << "Max: " << mx << std::endl;
  std::cout << "SD: " << sd << std::endl;

  if (cellwise)
    {
    const auto qualityArray =
      vtkDoubleArray::SafeDownCast(quality->GetOutput()->GetCellData()->GetArray("Quality"));
 
  std::cout << "# Number of cells: " << qualityArray->GetNumberOfTuples() << std::endl;
  std::cout << "CellID,Area\n";
 
  for (vtkIdType i = 0; i < qualityArray->GetNumberOfTuples(); ++i)
    {
    double val = qualityArray->GetValue(i);
    std::cout << i << ',' << val << '\n';
    }
    }
  return EXIT_SUCCESS;
 
}
