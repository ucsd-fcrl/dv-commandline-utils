#ifndef dv_GenerateInitialModel_h
#define dv_GenerateInitialModel_h

// STD
#include <string>

// ITK
#include <itkDefaultDynamicMeshTraits.h>
#include <itkMesh.h>
#include <itkQuadEdgeMesh.h>
#include <itkMeshFileReader.h>
#include <itkMeshFileWriter.h>
#include <itkDelaunayConformingQuadEdgeMeshFilter.h>
#include <itkAdditiveGaussianNoiseQuadEdgeMeshFilter.h>
#include <itkQuadEdgeMeshDecimationCriteria.h>
#include <itkSquaredEdgeLengthDecimationQuadEdgeMeshFilter.h>
#include <itkConnectedRegionsMeshFilter.h>
#include <itkLoopTriangleCellSubdivisionQuadEdgeMeshFilter.h>

// DV
#include <dvDeleteIsolatedPoints.h>
#include <dvSqueezePointsIds.h>
#include <dvRefineValenceThreeVertices.h>
#include <dvMeshToQuadEdgeMesh.h>

namespace dv
{

void
GenerateInitialModel(
  const std::string &inputMeshName,
  const std::string &outputMeshName,
  const unsigned int &count,
  const double &sigma
  )
{

const unsigned int Dimension = 3;
using TCoordinate = float;
 
using TMeshTraits = itk::DefaultDynamicMeshTraits< TCoordinate >;
using TMesh       = itk::Mesh< TCoordinate, Dimension, TMeshTraits >;
using TReader     = itk::MeshFileReader< TMesh >;

using TQEMesh       = itk::QuadEdgeMesh< TCoordinate, Dimension >;
using TQEWriter     = itk::MeshFileWriter< TQEMesh >;

using TConnected = itk::ConnectedRegionsMeshFilter< TMesh, TMesh >;
using TNoise = itk::AdditiveGaussianNoiseQuadEdgeMeshFilter< TQEMesh >;
using TCriterion  = itk::NumberOfFacesCriterion< TQEMesh >;
using TDecimation = itk::SquaredEdgeLengthDecimationQuadEdgeMeshFilter< TQEMesh,
                                                              TQEMesh,
                                                              TCriterion >;
using TLoop = itk::LoopTriangleCellSubdivisionQuadEdgeMeshFilter< TQEMesh, TQEMesh >;
using TDelaunay = itk::DelaunayConformingQuadEdgeMeshFilter< TQEMesh >;

const auto reader = TReader::New();
reader->SetFileName( inputMeshName );

const auto connected = TConnected::New();
connected->SetInput( reader->GetOutput() );
connected->SetExtractionModeToLargestRegion();
connected->Update();

const auto mesh = TMesh::New();
mesh->Graft( connected->GetOutput() );
mesh->DisconnectPipeline();

dv::DeleteIsolatedPoints<TMesh>( mesh );
dv::SqueezePointsIds<TMesh>( mesh );

const auto qemesh = TQEMesh::New();
dv::MeshToQuadEdgeMesh< TMesh, TQEMesh >( mesh, qemesh );

const auto noise = TNoise::New();
noise->SetInput( qemesh );
noise->SetSigma( sigma );

const auto criterion = TCriterion::New();
const auto decimate = TDecimation::New();

criterion->SetNumberOfElements( count );

decimate->SetInput( noise->GetOutput() );
decimate->SetCriterion( criterion );

const auto delaunay = TDelaunay::New();
delaunay->SetInput( decimate->GetOutput() );

const auto loop = TLoop::New();
loop->SetInput( delaunay->GetOutput() );

const auto writer = TQEWriter::New();
writer->SetInput( loop->GetOutput() );
writer->SetFileName( outputMeshName );
writer->Update();

}

}

#endif
