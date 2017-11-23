// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkImageFileReader.h>
#include <itkChangeInformationImageFilter.h>
#include <itkImageFileWriter.h>

const unsigned int Dimension = 3;
 
using TImage  = itk::Image<unsigned short, Dimension>;
using TReader = itk::ImageFileReader<TImage>;
using TInfo   = itk::ChangeInformationImageFilter<TImage>;
using TWriter = itk::ImageFileWriter<TImage>;

int
main(int argc, char ** argv)
{

  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-image",     po::value<std::string>()->required(), "Filename of the input image.")
    ("reference-image", po::value<std::string>()->required(), "Filename of the reference image.")
    ("output-image",    po::value<std::string>()->required(), "Filename of the output image.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const auto referenceReader = TReader::New();
  referenceReader->SetFileName( vm["reference-image"].as<std::string>() );
  referenceReader->Update();

  const auto reader = TReader::New();
  reader->SetFileName( vm["input-image"].as<std::string>() );

  const auto info = TInfo::New();
  info->SetInput( reader->GetOutput() );
  info->SetReferenceImage( referenceReader->GetOutput() );
  info->UseReferenceImageOn();
  info->ChangeAll();

  const auto writer = TWriter::New();
  writer->SetInput( info->GetOutput() );
  writer->SetFileName( vm["output-image"].as<std::string>() );
  writer->Update();
 
  return EXIT_SUCCESS;
}
