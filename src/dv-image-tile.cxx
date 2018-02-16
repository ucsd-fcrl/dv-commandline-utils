// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include "itkTileImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"

// Custom
#include <dvNumberOfSequentialFiles.h>
#include <dvCycle.h>
#include <dvStringOperations.h>
#include <dvProgress.h>



int main(int argc, char *argv[] )
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help",           "Print usage information.")
    ("input-images", po::value<std::vector<std::string>>()->multitoken()->required(), "Directory of the patient")
    ("output-image", po::value<std::string>()->required(), "Directory of the output images.")
    ("rows",         po::value<unsigned int>(), "Number of rows.")
    ("columns",      po::value<unsigned int>(), "Number of columns.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const auto InputImages = vm["input-images"].as<std::vector<std::string>>();
  const auto OutputImage = vm["output-image"].as<std::string>();
  const auto NumRows = vm["rows"].as<unsigned int>();
  const auto NumCols = vm["columns"].as<unsigned int>();

  typedef short  PixelType;
  const unsigned int InputImageDimension = 2;
  const unsigned int OutputImageDimension = 2;

  typedef itk::Image< PixelType, InputImageDimension  > InputImageType;
  typedef itk::Image< PixelType, OutputImageDimension > OutputImageType;
  typedef itk::ImageFileReader< InputImageType > ImageReaderType;
  typedef itk::TileImageFilter< InputImageType, OutputImageType > TilerType;
  typedef itk::ImageFileWriter< OutputImageType > WriterType;

  const auto tiler = TilerType::New();
  itk::FixedArray< unsigned int, OutputImageDimension > layout;
  layout[0] = NumCols;
  layout[1] = NumRows;
  layout[2] = 0;

  tiler->SetLayout( layout );

  unsigned int inputImageNumber = 0;

  ImageReaderType::Pointer reader = ImageReaderType::New();

  InputImageType::Pointer inputImageTile;

  std::cout << InputImages.size() << std::endl;

  for (int i = 0; i < InputImages.size(); i++)
    {
      reader->SetFileName( InputImages[i] );
      reader->Update();
      //reader->UpdateLargestPossibleRegion();
      inputImageTile = reader->GetOutput();
      inputImageTile->DisconnectPipeline();
      tiler->SetInput( inputImageNumber++, inputImageTile );
      //std::cout <<  inputImageNumber << std::endl;
    }


  PixelType filler = 128;
  tiler->SetDefaultPixelValue( filler );

  tiler->Update();

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( tiler->GetOutput() );
  writer->SetFileName( OutputImage );

  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }


  return EXIT_SUCCESS;
}
