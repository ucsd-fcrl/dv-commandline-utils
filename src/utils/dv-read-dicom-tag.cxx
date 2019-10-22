// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Custom
#include <dvReadDICOMTag.h>

int
main(int argc, char**argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-image", po::value<std::string>()->required(), "Filename of input image.")
    ("dicom-tag", po::value<std::string>()->required(), "DICOM tag in the format: 0028|0030");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc)
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const auto IImage = vm["input-image"].as<std::string>();
  const auto Tag = vm["dicom-tag"].as<std::string>();

  const auto pack = dv::ReadDICOMTag(IImage, Tag);

  if (std::get<0>(pack))
    {
    std::cout << std::get<2>(pack) << std::endl;
    return EXIT_SUCCESS;
    }
  else
    {
    std::cout << std::get<1>(pack) << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;

}
