/*=========================================================================
 *
 *  Copyright NumFOCUS
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
#ifndef itkRefineValenceThreeVerticesQuadEdgeMeshFilter_h
#define itkRefineValenceThreeVerticesQuadEdgeMeshFilter_h

#include "itkQuadEdgeMeshToQuadEdgeMeshFilter.h"

namespace itk
{
/** \class RefineValenceThreeVerticesQuadEdgeMeshFilter
 * \brief Remove Valence Three Vertices
 *
 * \author Davis Vigneault
 *
 * This class removes valence three vertices.
 *
 * \ingroup DVUtilities
 */
template <typename TInputMesh, typename TOutputMesh = TInputMesh>
class RefineValenceThreeVerticesQuadEdgeMeshFilter : public QuadEdgeMeshToQuadEdgeMeshFilter<TInputMesh, TOutputMesh>
{
public:
  ITK_DISALLOW_COPY_AND_ASSIGN(RefineValenceThreeVerticesQuadEdgeMeshFilter);

  /** Standard class type alias. */
  using Self = RefineValenceThreeVerticesQuadEdgeMeshFilter;
  using Superclass = QuadEdgeMeshToQuadEdgeMeshFilter<TInputMesh, TOutputMesh>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Type for representing coordinates. */
  using CoordRepType = typename TInputMesh::CoordRepType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(RefineValenceThreeVerticesQuadEdgeMeshFilter, QuadEdgeMeshToQuadEdgeMeshFilter);

protected:
  RefineValenceThreeVerticesQuadEdgeMeshFilter();
  ~RefineValenceThreeVerticesQuadEdgeMeshFilter() override = default;

  void
  PrintSelf(std::ostream & os, Indent indent) const override;

  /** Generate Requested Data */
  void
  GenerateData() override;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkRefineValenceThreeVerticesQuadEdgeMeshFilter.hxx"
#endif

#endif
