#ifndef itkEnforceBoundaryBetweenLabelsImageFilter_hxx
#define itkEnforceBoundaryBetweenLabelsImageFilter_hxx

#include "itkEnforceBoundaryBetweenLabelsImageFilter.h"

// ITK
#include <itkAndImageFilter.h>
#include <itkBinaryBallStructuringElement.h>
#include <itkBinaryDilateImageFilter.h>
#include <itkMaskImageFilter.h>

// Custom
#include "itkExtractLabelsImageFilter.h"

namespace itk
{

template <typename TInputImage, typename TOutputImage>
EnforceBoundaryBetweenLabelsImageFilter<TInputImage, TOutputImage>
::EnforceBoundaryBetweenLabelsImageFilter()
{
  this->m_Radius1 = 1;
  this->m_Radius2 = 1;
  this->m_Replacement = 0;
}


template <typename TInputImage, typename TOutputImage>
void
EnforceBoundaryBetweenLabelsImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


template <typename TInputImage, typename TOutputImage>
void
EnforceBoundaryBetweenLabelsImageFilter<TInputImage, TOutputImage>
::GenerateData()
{

  OutputImageType *      output = this->GetOutput();

  if ((0 == this->m_Radius1) && (0 == this->m_Radius2)) {
    std::cerr << "Radius1 and Radius2 are both set to zero.\n";
    std::cerr << "Returning output unchanged." << std::endl;
    output->Graft(this->GetInput());
    return;
  }

  if (0 == this->m_Labels1.size()) {
    std::cerr << "Labels1 is empty.\n";
    std::cerr << "Returning output unchanged." << std::endl;
    output->Graft(this->GetInput());
    return;
  }

  if (0 == this->m_Labels2.size()) {
    std::cerr << "Labels2 is empty.\n";
    std::cerr << "Returning output unchanged." << std::endl;
    output->Graft(this->GetInput());
    return;
  }

  using TFilter = itk::ExtractLabelsImageFilter<TInputImage>;
  using TBall = itk::BinaryBallStructuringElement<InputPixelType, InputImageDimension>;
  using TDilate = itk::BinaryDilateImageFilter<TInputImage, TInputImage, TBall>;
  using TAnd = itk::AndImageFilter<TInputImage>;
  using TMask = itk::MaskImageFilter<TInputImage, TInputImage>;

  //
  // make unary filter which checks if pixels are in set
  //
  const auto filter_1 = TFilter::New();
  filter_1->SetLabels(this->GetLabels1());
  filter_1->SetInput(this->GetInput());

  const auto filter_2 = TFilter::New();
  filter_2->SetLabels(this->GetLabels2());
  filter_2->SetInput(this->GetInput());

  // Dilate

  TBall ball_1;
  ball_1.SetRadius(this->GetRadius1());
  ball_1.CreateStructuringElement();

  const auto dilate_1 = TDilate::New();
  dilate_1->SetInput(filter_1->GetOutput());
  dilate_1->SetKernel(ball_1);
  dilate_1->SetDilateValue(1);

  TBall ball_2;
  ball_2.SetRadius(this->GetRadius2());
  ball_2.CreateStructuringElement();

  const auto dilate_2 = TDilate::New();
  dilate_2->SetInput(filter_2->GetOutput());
  dilate_2->SetKernel(ball_2);
  dilate_2->SetDilateValue(1);

  // Intersection
  const auto intersection = TAnd::New();
  intersection->SetInput1(dilate_1->GetOutput());
  intersection->SetInput2(dilate_2->GetOutput());
  intersection->Update();

  const auto mask = TMask::New();
  mask->SetInput(this->GetInput());
  mask->SetMaskImage(intersection->GetOutput());
  mask->SetMaskingValue(1);
  mask->SetOutsideValue(this->GetReplacement());
  mask->Update();

  output->Graft( mask->GetOutput() );

}

} // end namespace itk

#endif // itkEnforceBoundaryBetweenLabelsImageFilter_hxx
