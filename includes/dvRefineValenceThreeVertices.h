/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef dv_RefineValenceThreeVertices_h
#define dv_RefineValenceThreeVertices_h

#include <itkQuadEdgeMesh.h>
#include <itkQuadEdgeMeshEulerOperatorSplitEdgeFunction.h> 
#include <itkQuadEdgeMeshEulerOperatorSplitFacetFunction.h>

namespace dv
{
/**
 * \class RefineValenceThreeVertices
 * \brief
 *
 * Subdivide vertices of valence three.
 *
 * \ingroup ITKDVUtilities
 */

template< typename TMesh >
bool
MeshIncludesValenceThreeVertices( typename TMesh::Pointer mesh )
{
for (auto it = mesh->GetPoints()->Begin();
     it != mesh->GetPoints()->End();
     ++it)
  {
  if (3 == it->Value().GetValence())
    {
    return true;
    }
  }
  return false;
}

template< typename TMesh >
void
RefineValenceThreeVertices( typename TMesh::Pointer mesh )
{

typedef typename TMesh::QEType TQE;
typedef itk::QuadEdgeMeshEulerOperatorSplitEdgeFunction< TMesh, TQE >  TSplitEdge;
typedef itk::QuadEdgeMeshEulerOperatorSplitFacetFunction< TMesh, TQE > TSplitFacet;

std::vector<unsigned int> pt3;
for (auto it = mesh->GetPoints()->Begin();
     it != mesh->GetPoints()->End();
     ++it)
  {
  if (3 != it->Value().GetValence())
    {
    continue;
    }
  pt3.emplace_back(it->Index());
  }

  const auto splitEdge = TSplitEdge::New();
  const auto splitFace = TSplitFacet::New();

  for (const auto extraordinary_index : pt3)
    {
    const auto extraordinary_point = mesh->GetPoints()->ElementAt(extraordinary_index);

    auto edge = extraordinary_point.GetEdge();
    const auto edgeToSplit = edge->GetLnext();

    auto edgeToSplitOriginIndex = edgeToSplit->GetOrigin();
    auto edgeToSplitDestinationIndex = edgeToSplit->GetDestination();
    auto edgeToSplitOriginPoint = mesh->GetPoints()->ElementAt(edgeToSplitOriginIndex);
    auto edgeToSplitDestinationPoint = mesh->GetPoints()->ElementAt(edgeToSplitDestinationIndex);

    typename TMesh::PointType midPoint;
    midPoint.SetToMidPoint(edgeToSplitOriginPoint,
                           edgeToSplitDestinationPoint);

    splitEdge->SetInput( mesh );
    const auto newEdge = splitEdge->Evaluate( edgeToSplit->GetLnext() );
    const auto newPointDestinationIndex = newEdge->GetDestination();
    auto newPointDestinationPosition = mesh->GetPoints()->ElementAt(newPointDestinationIndex);
    for (unsigned int i = 0; i < 3; ++i)
      newPointDestinationPosition[i] = midPoint[i];
    mesh->SetPoint(newPointDestinationIndex, newPointDestinationPosition);

    const auto faceSplit1 = edge->GetLprev();
    const auto faceSplit2 = edge->GetLnext();
    const auto faceSplit3 = faceSplit2->GetSym()->GetLprev();
    const auto faceSplit4 = faceSplit3->GetLnext()->GetLnext();

    splitFace->SetInput( mesh );
    splitFace->Evaluate( faceSplit1, faceSplit2 );
    splitFace->Evaluate( faceSplit3, faceSplit4 );

    }

}

}

#endif
