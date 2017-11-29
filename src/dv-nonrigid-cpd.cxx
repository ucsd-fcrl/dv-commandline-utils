// STD
#include <iostream>

// ITK
#include <itkMesh.h>
#include <itkMeshFileReader.h>

// CPD
#include <cpd/nonrigid.hpp>

// Custom
#include "includes/dvITKPointSetToCPDMatrix.h"

const unsigned int Dimension = 3;
using TCoordinate = float;

using TMesh = itk::Mesh<TCoordinate, Dimension>;
using TMeshReader = itk::MeshFileReader<TMesh>;
using TMeshIt = TMesh::PointsContainer::Iterator;

int main(int argc, char** argv)
{

    if (3 != argc)
      {
      std::cerr << "ERROR: invalid usage" << std::endl;
      std::cerr << "USAGE: cpd-nonrigid <fixed> <moving>" << std::endl;
      return EXIT_FAILURE;
      }

    const std::string fFileName = "../testdata/suzanne.obj";
    const std::string mFileName = "../testdata/suzanne-shifted.obj";

    // Fixed
    const auto fReader = TMeshReader::New();
    fReader->SetFileName( fFileName );
    fReader->Update();

    const auto fMesh = TMesh::New();
    fMesh->Graft( fReader->GetOutput() );
    fMesh->DisconnectPipeline();

    const auto fMat = dv::ITKPointSetToCPDMatrix<TMesh>(fMesh);

    // Moving
    const auto mReader = TMeshReader::New();
    mReader->SetFileName( mFileName );
    mReader->Update();

    const auto mMesh = TMesh::New();
    mMesh->Graft( mReader->GetOutput() );
    mMesh->DisconnectPipeline();

    const auto mMat = dv::ITKPointSetToCPDMatrix<TMesh>(mMesh);

    cpd::NonrigidResult result = cpd::nonrigid(fMat, mMat);
    std::cout << result.points << std::endl;

    return EXIT_SUCCESS;

}
