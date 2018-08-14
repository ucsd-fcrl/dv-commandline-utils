// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Custom
#include <dvWriteArrowFromStartAndEnd.h>

int
main( int argc, char** argv )
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("start-point",  po::value<std::vector<double>>()->multitoken(), "Label map: 1 2 2 1 => {1 : 2, 2 : 1}.")
    ("end-point",  po::value<std::vector<double>>()->multitoken(),   "Label map: 1 2 2 1 => {1 : 2, 2 : 1}.")
    ("file-name",  po::value<std::string>()->required(),             "Filename of the input image.")
    ("tip-resolution",  po::value<int>()->default_value(25),         "Label map: 1 2 2 1 => {1 : 2, 2 : 1}.")
    ("shaft-resolution",  po::value<int>()->default_value(25),       "Filename of the input image.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const std::vector<double> startPoint = vm["start-point"].as<std::vector<double>>();
  const std::vector<double> endPoint = vm["end-point"].as<std::vector<double>>();
  const std::string fileName(vm["file-name"].as<std::string>());
  const int tipResolution = vm["tip-resolution"].as<int>();
  const int shaftResolution = vm["shaft-resolution"].as<int>();

  dv::ArrowWithStartAndEnd(startPoint.data(), endPoint.data(), fileName, tipResolution, shaftResolution);

  return EXIT_SUCCESS;

}
