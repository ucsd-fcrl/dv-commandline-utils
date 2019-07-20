// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

#include <dvReadImageIOBase.h>

int
main(int argc, char ** argv)
{

  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-image-1", po::value<std::string>()->required(), "Filename of input image 1.")
    ("input-image-2", po::value<std::string>()->required(), "Filename of input image 2.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || vm.empty())
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const auto Image1 = dv::ReadImageIOBase(vm["input-image-1"].as<std::string>());
  const auto Image2 = dv::ReadImageIOBase(vm["input-image-2"].as<std::string>());

  if (Image1->GetNumberOfDimensions() != Image2->GetNumberOfDimensions())
    {
    std::cerr << "Number of dimensions do not match." << std::endl;
    return EXIT_FAILURE;
    }

//  if (Image1->GetComponentSize() != Image2->GetComponentSize())
//    {
//    std::cerr << "Component sizes do not match." << std::endl;
//    return EXIT_FAILURE;
//    }
//
//  if (Image1->GetComponentType() != Image2->GetComponentType())
//    {
//    std::cerr << "Component types do not match." << std::endl;
//    return EXIT_FAILURE;
//    }

  for (size_t i = 0; i < Image1->GetNumberOfDimensions(); ++i)
    {
    if (Image1->GetDimensions(i) != Image2->GetDimensions(i))
      {
      std::cerr << "Matrix size does not match." << std::endl;
      return EXIT_FAILURE;
      }
    }

  for (size_t i = 0; i < Image1->GetNumberOfDimensions(); ++i)
    {
    const auto dir_1 = Image1->GetDirection(i);
    const auto dir_2 = Image2->GetDirection(i);
    for (size_t j = 0; j < dir_1.size(); ++j)
      {
      if (std::abs(dir_1[j] - dir_2[j]) > 1e-5)
        {
        std::cerr << "Direction matrices do not match." << std::endl;
        return EXIT_FAILURE;
        }
      }
    }

  for (size_t i = 0; i < Image1->GetNumberOfDimensions(); ++i)
    {
    if (std::abs(Image1->GetOrigin(i) - Image2->GetOrigin(i)) > 1e-5)
      {
      std::cerr << "Image origins do not match." << std::endl;
      return EXIT_FAILURE;
      }
    }

  for (size_t i = 0; i < Image1->GetNumberOfDimensions(); ++i)
    {
    const auto spacing_1 = Image1->GetSpacing(i);
    const auto spacing_2 = Image2->GetSpacing(i);
    if (std::abs(Image1->GetSpacing(i) - Image2->GetSpacing(i)) > 1e-5)
      {
      std::cerr << "Image spacing does not match." << std::endl;
      return EXIT_FAILURE;
      }
    }

  return EXIT_SUCCESS;
}
