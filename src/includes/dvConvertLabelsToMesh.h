#ifndef dv_ConvertLabelsToMesh_h
#define dv_ConvertLabelsToMesh_h

// ITK
#include <itkMesh.h>
#include <itkImageFileReader.h>
#include <itkMeshFileWriter.h>
#include <itkSTLMeshIO.h>
#include <itkBinaryMask3DMeshSource.h>

// Custom
#include "itkExtractLabelsImageFilter.h"

namespace dv
{

template<unsigned int Dimension, typename TPixel, typename TCoordinate>
void
ConvertLabelsToMesh(const std::string &IImage, const std::set<TPixel> &LabelSet, const std::string &OMesh)
{

  using TImage = itk::Image< TPixel, Dimension >;
  using TMesh   = itk::Mesh< TCoordinate, Dimension >;

  using ReaderType = itk::ImageFileReader< TImage >;
  using FilterType = itk::ExtractLabelsImageFilter<TImage>;
  using TMeshSource = itk::BinaryMask3DMeshSource< TImage, TMesh >;

  using TWriter = itk::MeshFileWriter< TMesh >;

  //
  // Reader
  //

  const auto reader = ReaderType::New();
  reader->SetFileName( IImage );

  //
  // make unary filter which checks if pixels are in set
  //
  const auto filter = FilterType::New();
  filter->SetLabels( LabelSet );
  filter->SetInput( reader->GetOutput() );

  //
  // Meshing
  //
  const auto meshSource = TMeshSource::New();
  const TPixel objectValue = static_cast<TPixel>( 1 );
  meshSource->SetObjectValue( objectValue );
  meshSource->SetInput( filter->GetOutput() );

  //
  // Writer
  //

  const auto writer = TWriter::New();
  writer->SetInput( meshSource->GetOutput() );
  writer->SetFileName( OMesh );

  const auto o_ext = OMesh.substr( OMesh.size() - 3, 3);
  if (o_ext == "stl" || o_ext == "STL")
  {
    writer->SetMeshIO( itk::STLMeshIO::New() );
  }

  writer->Update();

}

}

#endif
