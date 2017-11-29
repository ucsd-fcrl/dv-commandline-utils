// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Custom
#include <dvBinarizeMesh.h>

const unsigned int Dimension = 3;
using ImagePixelType = unsigned char;

int main(int argc, char** argv)
{

  std::string iMesh, rImage, oImage;

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-mesh",      po::value<std::string>(&iMesh)->required(), "Filename of the input mesh.")
    ("reference-image", po::value<std::string>(&rImage)->required(), "Filename of the reference image.")
    ("output-image",    po::value<std::string>(&oImage)->required(), "Filename of the output image.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  dv::BinarizeMesh< Dimension, double, ImagePixelType >(iMesh,
                                                        rImage,
                                                        oImage);

  return EXIT_SUCCESS;
}
