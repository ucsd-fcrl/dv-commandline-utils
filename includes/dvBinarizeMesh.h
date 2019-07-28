#ifndef dv_BinarizeMesh_h
#define dv_BinarizeMesh_h

// ITK
#include <itkMesh.h>
#include <itkMeshFileReader.h>

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

#include <itkTriangleMeshToBinaryImageFilter.h>

namespace dv
{

template<unsigned int Dimension, typename MeshPixelType, typename ImagePixelType>
void
BinarizeMesh(const std::string &iMesh,
             const std::string &rImage,
             const std::string &oImage)
{

  using MeshType = itk::Mesh< MeshPixelType, Dimension >;

  using MeshReaderType = itk::MeshFileReader< MeshType >;

  using ImageType = itk::Image< ImagePixelType, Dimension >;
  using ImageReaderType = itk::ImageFileReader< ImageType >;

  using FilterType = itk::TriangleMeshToBinaryImageFilter< MeshType, ImageType >;

  using WriterType = itk::ImageFileWriter< ImageType >;

  const auto meshReader = MeshReaderType::New();
  meshReader->SetFileName( iMesh );


  const auto imageReader = ImageReaderType::New();
  imageReader->SetFileName( rImage );

  const auto filter = FilterType::New();
  filter->SetInput( meshReader->GetOutput() );
  filter->SetInfoImage( imageReader->GetOutput() );
  filter->SetInsideValue( 1 );
  filter->Update();

  const auto writer = WriterType::New();
  writer->SetFileName( oImage );
  writer->SetInput( filter->GetOutput() );
  writer->Update();

}

}

#endif
