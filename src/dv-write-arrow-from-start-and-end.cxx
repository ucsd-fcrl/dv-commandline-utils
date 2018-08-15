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
    ("point-1", po::value<std::vector<double>>()->multitoken(), "Starting point.")
    ("point-2", po::value<std::vector<double>>()->multitoken(), "Ending point.")
    ("file-name", po::value<std::string>()->required(), "Filename of mesh.")
    ("tip-resolution",  po::value<int>()->default_value(25), "Tip resolution.")
    ("shaft-resolution",  po::value<int>()->default_value(25), "Shaft resolution.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const std::vector<double> point1 = vm["point-1"].as<std::vector<double>>();
  const std::vector<double> point2 = vm["point-2"].as<std::vector<double>>();
  assert(point1.size() == 3);
  assert(point2.size() == 3);
  const std::string fileName(vm["file-name"].as<std::string>());
  const int tipResolution = vm["tip-resolution"].as<int>();
  const int shaftResolution = vm["shaft-resolution"].as<int>();

  dv::ArrowWithStartAndEnd(point1.data(), point2.data(), fileName, tipResolution, shaftResolution);

  return EXIT_SUCCESS;

}
