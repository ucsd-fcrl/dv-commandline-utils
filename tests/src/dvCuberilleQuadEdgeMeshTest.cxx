// ITK
#include <itkTestingMacros.h>
#include <itkImage.h>
#include <itkConstantPadImageFilter.h>
#include <itkQuadEdgeMesh.h>
#include <itkCuberilleImageToMeshFilter.h>

int main() {

  using TPixel = unsigned char;
  using TCoordinate = float;
  const unsigned int Dimension = 3;
  using TImage = itk::Image<TPixel, Dimension>;
  using TMesh = itk::QuadEdgeMesh<TCoordinate, Dimension>;
  using TPad = itk::ConstantPadImageFilter<TImage, TImage>;
  using TExtract = itk::CuberilleImageToMeshFilter< TImage, TMesh >;

  for (size_t mask = 0; mask < std::pow(2, 8); ++mask) {

    std::bitset<8> bitmask(mask);

    std::cout << mask << ' ' << bitmask << std::endl;

    const auto image = TImage::New();
    TImage::SizeType size;
    size.Fill( 2 );

    TImage::IndexType origin;
    origin.Fill( 0 );

    TImage::RegionType region(origin, size);

    image->SetRegions( region );
    image->Allocate();
    image->FillBuffer( 0 );

    for (size_t index = 0; index < std::pow(2, 3); ++index) {
      std::bitset<3> bitindex(index);
      image->SetPixel( {{bitindex[0], bitindex[1], bitindex[2]}}, bitmask[index] );
    }

    typename TImage::SizeType padding;
    padding.Fill(1);

    const auto pad = TPad::New();
    pad->SetInput(image);
    pad->SetPadUpperBound(padding);
    pad->SetPadLowerBound(padding);
    pad->SetConstant(static_cast<TPixel>(0));

    const auto extract = TExtract::New();
    extract->SetInput( pad->GetOutput() );
    extract->GenerateTriangleFacesOn();
    extract->ProjectVerticesToIsoSurfaceOff();
    extract->SavePixelAsCellDataOn();
    ITK_TRY_EXPECT_NO_EXCEPTION(extract->Update());

  }

  return EXIT_SUCCESS;

}
