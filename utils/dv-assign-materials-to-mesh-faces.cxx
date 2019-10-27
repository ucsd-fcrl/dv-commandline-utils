// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkMeshFileReader.h>
#include <itkQuadEdgeMesh.h>
#include <itkTriangleHelper.h>

#include <itkImage.h>
#include <itkImageFileReader.h>

// Typedefs
const unsigned int Dimension = 3;
using TCoordinate = float;

using TImage = itk::Image<TCoordinate, Dimension>;
using TImageReader = itk::ImageFileReader<TImage>;

using TMesh = itk::QuadEdgeMesh<TCoordinate, Dimension>;
using TMeshReader = itk::MeshFileReader<TMesh>;
using TTriangle = itk::TriangleCell<TMesh::CellType>;
using TTriangleHelper = itk::TriangleHelper<TMesh::PointType>;

int
main(int argc, char** argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()("help", "Print usage information.")(
    "input-mesh",
    po::value<std::string>()->required(),
    "Filename of the input mesh.")("input-image",
                                   po::value<std::string>()->required(),
                                   "Filename of the reference segmentation.")(
    "output-mesh",
    po::value<std::string>()->required(),
    "Filename of the output mesh.");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc) {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
  }

  po::notify(vm);

  const std::string inputMeshName(vm["input-mesh"].as<std::string>());
  const std::string inputImageName(vm["input-image"].as<std::string>());
  const std::string outputMeshName(vm["output-mesh"].as<std::string>());

  const auto imageReader = TImageReader::New();
  imageReader->SetFileName(inputImageName);
  imageReader->Update();

  const auto image = TImage::New();
  image->Graft(imageReader->GetOutput());

  const auto meshReader = TMeshReader::New();
  meshReader->SetFileName(inputMeshName);
  meshReader->Update();

  const auto mesh = TMesh::New();
  mesh->Graft(meshReader->GetOutput());

  std::ostringstream output_stream;

  for (auto it = mesh->GetPoints()->Begin(); it != mesh->GetPoints()->End();
       ++it) {
    output_stream << "v " << it.Value()[0] << ' ' << it.Value()[1] << ' '
                  << it.Value()[2] << '\n';
  }

  std::map<unsigned int, std::vector<unsigned int>> label_map;

  for (auto it = mesh->GetCells()->Begin(); it != mesh->GetCells()->End();
       ++it) {
    const auto point_ids_for_cell = it.Value()->GetPointIds();

    TMesh::PointType p0;
    TMesh::PointType p1;
    TMesh::PointType p2;

    mesh->GetPoint(point_ids_for_cell[0], &p0);
    mesh->GetPoint(point_ids_for_cell[0], &p1);
    mesh->GetPoint(point_ids_for_cell[0], &p2);

    const auto centerpoint = TTriangleHelper::ComputeGravityCenter(p0, p1, p2);

    TImage::IndexType index;
    image->TransformPhysicalPointToIndex(centerpoint, index);
    const TCoordinate pixel = image->GetPixel(index);

    label_map[pixel].emplace_back(it.Index());
  }

  for (auto l : label_map) {
    output_stream << "usemtl material-" << l.first << '\n';
    for (auto i : l.second) {
      const auto f = mesh->GetCells()->GetElement(i)->GetPointIds();
      output_stream << "f " << (f[0] + 1) << ' ' << (f[1] + 1) << ' '
                    << (f[2] + 1) << '\n';
    }
  }

  std::ofstream output_file(outputMeshName);
  output_file << output_stream.str();
}
