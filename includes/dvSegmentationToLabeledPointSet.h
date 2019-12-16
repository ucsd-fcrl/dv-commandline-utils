#ifndef dv_SegmentationToLabeledPointSet_h
#define dv_SegmentationToLabeledPointSet_h

// STD
#include <fstream>

// ITK
#include <itkPointSet.h>
#include <itkMesh.h>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkConstantPadImageFilter.h>
#include <itkNearestNeighborInterpolateImageFunction.h>
#include <itkPointSet.h>
#include <itkMesh.h>
#include <itkCuberilleImageToMeshFilter.h>
#include <itkTriangleCell.h>
#include <itkTriangleHelper.h>

// Custom
#include <dvLabeledITKPointSetWriter.h>

namespace dv {

template<unsigned int Dimension, typename TPixel, typename TCoordinate>
void
SegmentationToLabeledPointSet(const std::string& input_image, const std::string& labeled_point_set) {

  using TImage = itk::Image<TPixel, Dimension>;
  using TMesh = itk::Mesh<TCoordinate, Dimension>;
  using TPointSet = itk::PointSet<TCoordinate, Dimension>;
  using TReader = itk::ImageFileReader<TImage>;
  using TPad = itk::ConstantPadImageFilter<TImage, TImage>;
  using TBinarize = itk::BinaryThresholdImageFilter< TImage, TImage >;
  using TInterp = itk::NearestNeighborInterpolateImageFunction< TImage >;
  using TMesh = itk::Mesh<double, TImage::ImageDimension>;
  using TExtract = itk::CuberilleImageToMeshFilter< TImage, TMesh >;
  using TTriangleHelper = itk::TriangleHelper< typename TMesh::PointType >;

  const auto reader = TReader::New();
  reader->SetFileName( input_image );
  reader->Update();

  const auto image = TImage::New();
  image->Graft( reader->GetOutput() );
  typename TImage::SizeType padding;
  padding.Fill(1);

  const auto pad = TPad::New();
  pad->SetInput(image);
  pad->SetPadUpperBound(padding);
  pad->SetPadLowerBound(padding);
  pad->SetConstant(static_cast<TPixel>(0));

  const auto binarize = TBinarize::New();
  binarize->SetInput( pad->GetOutput() );
  binarize->SetInsideValue( 1 );
  binarize->SetOutsideValue( 0 );
  binarize->SetLowerThreshold( 1 );

  const auto extract = TExtract::New();
  extract->SetInput( binarize->GetOutput() );
  extract->GenerateTriangleFacesOff();
  extract->ProjectVerticesToIsoSurfaceOff();
  extract->Update();

  const auto mesh = TMesh::New();
  mesh->Graft( extract->GetOutput() );

  const auto interp = TInterp::New();
  interp->SetInputImage( image );

  const auto half_spacing = image->GetSpacing() * 0.5;

  const auto points = TPointSet::New();

  for (auto it = mesh->GetCells()->Begin();
       it != mesh->GetCells()->End();
       ++it) {

    const auto cell = it.Value();

    typename TImage::PointType centroid;
    centroid.SetToMidPoint(
      mesh->GetPoint(cell->GetPointIds()[0]),
      mesh->GetPoint(cell->GetPointIds()[2])
      );

    auto normal = TTriangleHelper::ComputeNormal(
      mesh->GetPoint(cell->GetPointIds()[0]),
      mesh->GetPoint(cell->GetPointIds()[1]),
      mesh->GetPoint(cell->GetPointIds()[2])
    );

    normal.Normalize();

    for (size_t i = 0; i < 3; ++i) {
      normal[i] *= half_spacing[i];
    }

    const auto resample = centroid + normal;
    const auto label = interp->Evaluate( resample );

    itkAssertOrThrowMacro(label != 0, "Label == 0");

    points->SetPoint( it.Index(), centroid );
    points->SetPointData( it.Index(), label );

  }

  LabeledITKPointSetWriter<TPointSet>(points, labeled_point_set);

}
}

#endif
