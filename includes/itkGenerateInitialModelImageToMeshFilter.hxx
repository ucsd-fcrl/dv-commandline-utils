#ifndef itkGenerateInitialModelImageToMeshFilter_hxx
#define itkGenerateInitialModelImageToMeshFilter_hxx

#include "itkGenerateInitialModelImageToMeshFilter.h"

// ITK
#include <itkBinaryBallStructuringElement.h>
#include <itkBinaryMorphologicalClosingImageFilter.h>
#include <itkConstantPadImageFilter.h>
#include <itkCuberilleImageToMeshFilter.h>
#include <itkQuadEdgeMesh.h>
#include <itkAdditiveGaussianNoiseQuadEdgeMeshFilter.h>
#include <itkQuadEdgeMeshDecimationCriteria.h>
#include <itkSquaredEdgeLengthDecimationQuadEdgeMeshFilter.h>
#include <itkDelaunayConformingQuadEdgeMeshFilter.h>
#include <itkLoopTriangleCellSubdivisionQuadEdgeMeshFilter.h>

// Custom
#include <itkEnforceBoundaryBetweenLabelsImageFilter.h>
#include <itkFillHolesInSegmentationImageFilter.h>
#include <itkExtractConnectedComponentsImageFilter.h>

namespace itk
{

template <typename TInputImage, typename TOutputMesh>
GenerateInitialModelImageToMeshFilter<TInputImage, TOutputMesh>
::GenerateInitialModelImageToMeshFilter()
{
  this->SetNumberOfRequiredInputs(1);
}

template <typename TInputImage, typename TOutputMesh>
void
GenerateInitialModelImageToMeshFilter<TInputImage, TOutputMesh>
::SetInput(const InputImageType * image)
{
  this->ProcessObject::SetNthInput(0, const_cast<InputImageType *>(image));
}

template <typename TInputImage, typename TOutputMesh>
void
GenerateInitialModelImageToMeshFilter<TInputImage, TOutputMesh>
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


template <typename TInputImage, typename TOutputMesh>
void
GenerateInitialModelImageToMeshFilter<TInputImage, TOutputMesh>
::GenerateData()
{

  const auto image = Superclass::GetInput(0);
  typename OutputMeshType::Pointer mesh = Superclass::GetOutput();

  using TKernel = itk::BinaryBallStructuringElement<InputPixelType, InputImageDimension>;
  using TClose = itk::BinaryMorphologicalClosingImageFilter<InputImageType, InputImageType, TKernel>;
  using TFill = itk::FillHolesInSegmentationImageFilter<InputImageType>;
  using TEnforce = itk::EnforceBoundaryBetweenLabelsImageFilter<InputImageType>;
  using TConnected = itk::ExtractConnectedComponentsImageFilter<InputImageType>;
  using TPad = itk::ConstantPadImageFilter<InputImageType, InputImageType>;
  using TCuberille = itk::CuberilleImageToMeshFilter< InputImageType, OutputMeshType >;
  using TNoise = itk::AdditiveGaussianNoiseQuadEdgeMeshFilter<OutputMeshType>;
  using TCriterion = itk::NumberOfFacesCriterion<OutputMeshType>;
  using TDecimation = itk::SquaredEdgeLengthDecimationQuadEdgeMeshFilter<OutputMeshType, OutputMeshType, TCriterion>;
  using TLoop = itk::LoopTriangleCellSubdivisionQuadEdgeMeshFilter<OutputMeshType>;

  const auto enforce0 = TEnforce::New();
  enforce0->SetInput( image );
  enforce0->SetLabels1({4, 5});
  enforce0->SetLabels2({2, 3, 6, 7, 8, 9});

  const auto enforce1 = TEnforce::New();
  enforce1->SetInput( enforce0->GetOutput() );
  enforce1->SetLabels1({3});
  enforce1->SetLabels2({1, 6, 7, 8, 9});

  TKernel closeKernel;
  closeKernel.SetRadius(this->GetLVClosingRadius());
  closeKernel.CreateStructuringElement();

  const auto closing = TClose::New();
  closing->SetInput( enforce1->GetOutput() );
  closing->SetKernel( closeKernel );
  closing->SetForegroundValue( 1 );

  const auto connected = TConnected::New();
  connected->SetInput( closing->GetOutput() );

  const auto fill = TFill::New();
  fill->SetInput( connected->GetOutput() );

  typename InputImageType::SizeType padding;
  padding.Fill(1);

  const auto pad = TPad::New();
  pad->SetInput(fill->GetOutput());
  pad->SetPadUpperBound(padding);
  pad->SetPadLowerBound(padding);
  pad->SetConstant(static_cast<InputPixelType>(0));

  const auto cuberille = TCuberille::New();
  cuberille->SetInput( pad->GetOutput() );
  cuberille->GenerateTriangleFacesOn();
  cuberille->ProjectVerticesToIsoSurfaceOff();
  cuberille->SavePixelAsCellDataOn();

  const auto noise = TNoise::New();
  noise->SetInput(cuberille->GetOutput());
  noise->SetSigma(this->GetMeshNoiseSigma());

  const auto criterion = TCriterion::New();
  const auto decimate = TDecimation::New();

  criterion->SetNumberOfElements(this->GetNumberOfCellsInDecimatedMesh());

  decimate->SetInput(noise->GetOutput());
  decimate->SetCriterion(criterion);

  const auto loop = TLoop::New();
  loop->SetInput( decimate->GetOutput() );
  loop->Update();

  mesh->Graft( loop->GetOutput() );

}

} // end namespace itk

#endif // itkGenerateInitialModelImageToMeshFilter_hxx
