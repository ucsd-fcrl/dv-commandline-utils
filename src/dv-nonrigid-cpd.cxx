// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// STD
#include <iostream>

// ITK
#include <itkMesh.h>
#include <itkMeshFileReader.h>
#include <itkMeshFileWriter.h>
#include <itkSTLMeshIO.h>

// CPD
#include <cpd/nonrigid.hpp>

// Custom
#include <dvITKPointSetToCPDMatrix.h>
#include <dvUpdateITKPointSetWithCPDMatrix.h>
#include <dvGetLowercaseFileExtension.h>

const unsigned int Dimension = 3;
using TCoordinate = float;

using TMesh = itk::Mesh<TCoordinate, Dimension>;
using TMeshReader = itk::MeshFileReader<TMesh>;
using TMeshWriter = itk::MeshFileWriter<TMesh>;

int main(int argc, char** argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    // Filenames
    ("fixed-mesh",     po::value<std::string>()->required(),     "Filename of the fixed input mesh.")
    ("moving-mesh",    po::value<std::string>()->required(),     "Filename of the moving input mesh.")
    ("output-mesh",    po::value<std::string>()->required(),     "Filename of the output mesh.")

    // Defined in cpd/transform.hpp
    ("max-iterations", po::value<size_t>()->default_value(150),  "The default number of iterations allowed.")
    ("normalize",      po::value<bool>()->default_value(true),   "Whether points should be normalized by default.")
    ("outliers",       po::value<double>()->default_value(0.1),  "The default outlier weight.")
    ("tolerance",      po::value<double>()->default_value(1e-5), "The default tolerance.")
    ("correspondence", po::value<bool>()->default_value(false),  "Whether correspondence vector should be computed by default.")
    ("linked",         po::value<bool>()->default_value(true),   "Are the scalings of the two datasets linked by default?")

    // Defined in cpd/nonrigid.hpp
    ("beta",           po::value<double>()->default_value(3.0), "Value of beta coefficient.")
    ("lambda",         po::value<double>()->default_value(3.0), "Value of lambda coefficient.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const std::string fFileName = vm["fixed-mesh"].as<std::string>();
  const std::string mFileName = vm["moving-mesh"].as<std::string>();
  const std::string oFileName = vm["output-mesh"].as<std::string>();

  // Fixed
  const auto fReader = TMeshReader::New();
  fReader->SetFileName( fFileName );
  if (".stl" == dv::GetLowercaseFileExtension(fFileName))
    {
    fReader->SetMeshIO( itk::STLMeshIO::New() );
    }
  fReader->Update();

  const auto fMesh = TMesh::New();
  fMesh->Graft( fReader->GetOutput() );
  fMesh->DisconnectPipeline();

  const auto fMat = dv::ITKPointSetToCPDMatrix<TMesh>(fMesh);

  // Moving
  const auto mReader = TMeshReader::New();
  if (".stl" == dv::GetLowercaseFileExtension(mFileName))
    {
    mReader->SetMeshIO( itk::STLMeshIO::New() );
    }
  mReader->SetFileName( mFileName );
  mReader->Update();

  const auto mMesh = TMesh::New();
  mMesh->Graft( mReader->GetOutput() );
  mMesh->DisconnectPipeline();

  const auto mMat = dv::ITKPointSetToCPDMatrix<TMesh>(mMesh);

  cpd::Nonrigid nonrigid;
  nonrigid.max_iterations( vm["max-iterations"].as<size_t>() );
  nonrigid.normalize( vm["normalize"].as<bool>() );
  nonrigid.outliers( vm["outliers"].as<double>() );
  nonrigid.tolerance( vm["tolerance"].as<double>() );
  nonrigid.correspondence( vm["correspondence"].as<bool>() );
  nonrigid.linked( vm["linked"].as<bool>() );
  nonrigid.beta( vm["beta"].as<double>() );
  nonrigid.lambda( vm["lambda"].as<double>() );

  cpd::NonrigidResult result = nonrigid.run(fMat, mMat);

  std::cout << "Runtime (ms): "    << result.runtime.count() << std::endl;
  std::cout << "Iterations: " << result.iterations << std::endl;

  // Output
  const auto oMesh = TMesh::New();
  oMesh->Graft( mReader->GetOutput() );
  oMesh->DisconnectPipeline();

  dv::UpdateITKPointSetWithCPDMatrix<TMesh>(oMesh, result.points);

  const auto writer = TMeshWriter::New();
  if (".stl" == dv::GetLowercaseFileExtension(oFileName))
    {
    writer->SetMeshIO( itk::STLMeshIO::New() );
    }
  writer->SetInput( oMesh );
  writer->SetFileName( oFileName );
  writer->Update();

  return EXIT_SUCCESS;

}
