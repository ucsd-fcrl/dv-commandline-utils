// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkImageFileReader.h>
#include <itkPermuteAxesImageFilter.h>
#include <itkImageFileWriter.h>

const unsigned int Dimension = 3;
using TPixel = short;
 
using TImage = itk::Image<TPixel, Dimension>;

using TReader  = itk::ImageFileReader<TImage>;
using TPermute = itk::PermuteAxesImageFilter<TImage>;
using TWriter  = itk::ImageFileWriter<TImage>;

int
main(int argc, char ** argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-image",  po::value<std::string>()->required(),                             "Filename of the input image.")
    ("output-image", po::value<std::string>()->required(),                             "Filename of the output image.")
    ("order",        po::value<std::vector<unsigned int>>()->multitoken()->required(), "Desired axis order.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc)
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const std::string IImage = vm["input-image"].as<std::string>();
  const std::string OImage = vm["output-image"].as<std::string>();
  const std::vector<unsigned int> orderVec = vm["order"].as<std::vector<unsigned int>>();

  itk::FixedArray<unsigned int, Dimension> order;

  for (unsigned int i = 0; i < Dimension; ++i)
    {
    order[i] = orderVec.at(i);
    }  

  const auto reader = TReader::New();
  reader->SetFileName( IImage );

  const auto permute = TPermute::New();
  permute->SetInput( reader->GetOutput() );
  permute->SetOrder(order);

  const auto writer = TWriter::New();
  writer->SetInput( permute->GetOutput() );
  writer->SetFileName( OImage );
  writer->Update();
 
  return EXIT_SUCCESS;
}
