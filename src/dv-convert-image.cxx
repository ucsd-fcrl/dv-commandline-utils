// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

namespace dv
{

template<unsigned int Dimension, typename TPixel>
void
threshold(po::variables_map vm)
{

  using TImage = itk::Image<TPixel, Dimension>;
  using TReader = itk::ImageFileReader<TImage>;
  using TWriter = itk::ImageFileWriter<TImage>;
 
  const auto reader = TReader::New();
  reader->SetFileName( vm["input-image"].as<std::string>() );
 
  const auto writer = TWriter::New();
  writer->SetInput( reader->GetOutput() );
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
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  dv::threshold<3, signed short>(vm);

  return EXIT_SUCCESS;
}
