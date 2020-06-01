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
#ifndef itkExtractConnectedComponentsImageFilter_hxx
#define itkExtractConnectedComponentsImageFilter_hxx

#include "itkExtractConnectedComponentsImageFilter.h"

// ITK
#include <itkBinaryThresholdImageFilter.h>
#include <itkConnectedComponentImageFilter.h>
#include <itkLabelShapeKeepNObjectsImageFilter.h>
#include <itkMaskImageFilter.h>

// Custom
#include <dvImageToSet.h>

namespace itk
{

template <typename TInputImage, typename TOutputImage>
ExtractConnectedComponentsImageFilter<TInputImage, TOutputImage>
::ExtractConnectedComponentsImageFilter()
{
  this->m_N = 1;
}


template <typename TInputImage, typename TOutputImage>
void
ExtractConnectedComponentsImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


template <typename TInputImage, typename TOutputImage>
void
ExtractConnectedComponentsImageFilter<TInputImage, TOutputImage>
::GenerateData()
{

  using TLabel = itk::ConnectedComponentImageFilter<InputImageType, InputImageType>;
  using TKeep = itk::LabelShapeKeepNObjectsImageFilter<InputImageType>;
  using TThresh = itk::BinaryThresholdImageFilter<InputImageType, InputImageType>;
  using TMask = itk::MaskImageFilter<InputImageType, OutputImageType>;

  const InputImageType * input = this->GetInput();
  OutputImageType *      output = this->GetOutput();

  output->Graft( input );

  const auto labels_set = dv::ImageToSet<InputImageDimension, InputPixelType>(input);

  for (const auto& s : labels_set) {
    // Ignore background.
    if (0 == s)
      continue;

    // Extract Label:
    const auto thresh = TThresh::New();
    thresh->SetInput(output);
    thresh->SetLowerThreshold(s);
    thresh->SetUpperThreshold(s);
    thresh->SetInsideValue(s);
    thresh->SetOutsideValue(0);

    // Connected Components:
    const auto connected = TLabel::New();
    connected->SetInput(thresh->GetOutput());
    connected->FullyConnectedOn();
    connected->Update();

    const auto NumObjects = connected->GetObjectCount();

    // Continue if only N objects are detected.
    if (NumObjects <= this->GetN())
      continue;

    // Keep (ObjectCount - 1) smallest components.
    const auto keep = TKeep::New();
    keep->SetInput(connected->GetOutput());
    keep->SetBackgroundValue(0);
    keep->SetNumberOfObjects(NumObjects - this->GetN());
    keep->SetAttribute(TKeep::LabelObjectType::NUMBER_OF_PIXELS);
    keep->ReverseOrderingOn();

    // Turn it into a binary
    const auto thresh2 = TThresh::New();
    thresh2->SetInput(keep->GetOutput());
    thresh2->SetLowerThreshold(1);
    thresh2->SetInsideValue(1);
    thresh2->SetOutsideValue(0);

    // Mask the input
    const auto mask = TMask::New();
    mask->SetInput(input);
    mask->SetMaskImage(thresh2->GetOutput());
    mask->SetMaskingValue(1);
    mask->Update();

    // Replace the input
    output->Graft(mask->GetOutput());
    output->DisconnectPipeline();
  }

}

} // end namespace itk

#endif // itkExtractConnectedComponentsImageFilter_hxx
