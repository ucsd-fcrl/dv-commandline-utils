// Boost
#include <boost/program_options.hpp>

// ITK
#include <itkImageFileReader.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkImageFileWriter.h>

const unsigned int Dimension = 3;

namespace po = boost::program_options;

namespace dv
{

template<unsigned int Dimension, typename TPixel>
void
threshold(po::variables_map vm)
{

  using TImage = itk::Image<TPixel, Dimension>;
  using TReader = itk::ImageFileReader<TImage>;
  using TFilter = itk::BinaryThresholdImageFilter<TImage, TImage>;
  using TWriter = itk::ImageFileWriter<TImage>;
 
  const auto reader = TReader::New();
  reader->SetFileName( vm["input-image"].as<std::string>() );
 
  const auto filter = TFilter::New();
  filter->SetInput(reader->GetOutput());

  if (vm.count("interior-value"))
    {
    filter->SetInsideValue(vm["interior-value"].as<TPixel>());
    }

  if (vm.count("exterior-value"))
    {
    filter->SetOutsideValue(vm["exterior-value"].as<TPixel>());
    }

  if (vm.count("lower-threshold"))
    {
    filter->SetLowerThreshold(vm["lower-threshold"].as<TPixel>());
    }

  if (vm.count("upper-threshold"))
    {
    filter->SetUpperThreshold(vm["upper-threshold"].as<TPixel>());
    }

  const auto writer = TWriter::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( vm["output-image"].as<std::string>() );
  writer->Update();

}

}

int
main(int argc, char **argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-image", po::value<std::string>()->required(), "Filename of input image.")
    ("output-image", po::value<std::string>()->required(), "Filename of output image.")
    ("interior-value", po::value<signed short>(), "Value to set interior pixels.")
    ("exterior-value", po::value<signed short>(), "Value to set exterior pixels.")
    ("lower-threshold", po::value<signed short>(), "Lower threshold.")
    ("upper-threshold", po::value<signed short>(), "Upper threshold.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  dv::threshold<Dimension, signed short>(vm);

  return EXIT_SUCCESS;
}
