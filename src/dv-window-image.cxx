// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkFixedArray.h>
#include <itkImage.h>
#include <itkImageFileWriter.h>
#include <itkIntensityWindowingImageFilter.h>
#include <itkImageRegionIterator.h>
#include <itkNumericTraits.h>

// Custom
#include <dvReadImageIOBase.h>
#include <dvWindowImage.h>

int
main(int argc, char ** argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
  ("help", "Print usage information.")
  ("input-image",  po::value<std::string>()->required(),                             "Filename of the input mesh.")
  ("output-image", po::value<std::string>()->required(),                             "Filename of the output image.")
  ("scale",        po::value<std::vector<unsigned int>>()->multitoken()->required(), "Min and max values for window scaling")
  ("output-range", po::value<std::vector<unsigned int>>(), "Min and max values for output image")
;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
  {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
  }

  po::notify(vm);

  const std::string IImage = vm["input-image"].as<std::string>();
  const std::string OImage = vm["output-image"].as<std::string>();
  const std::vector<signed int> scaleVec = vm["scale"].as<std::vector<signed int>>();
  const std::vector<signed int> outputVec = vm["output-range"].as<std::vector<signed int>>();


  constexpr signed int DIM = 2;

  TArray scale;
  for (unsigned int i = 0; i < 2; ++i)
  {
    scale[i] = scaleVec.at(i);
  }

  const bool output_range_exists = vm.count("output-range");
  if(output_range_exists)
  {
    TArray o_range;
    for (unsigned int i = 0; i < 2; ++i)
    {
      o_range[i] = outputVec.at(i);
    }
  }

  switch (dv::ReadImageIOBase(IImage)->GetComponentType())
  {
    case itk::ImageIOBase::UCHAR:
    {
      if(!output_range_exists)
      {
        o_range[0]=itk::NumericTraits<unsigned char>::min();
        o_range[1]=itk::NumericTraits<unsigned char>::min();
      }
      dv::WindowImage<DIM, unsigned char>(IImage, OImage, scale, o_range);
      break;
    }

    case itk::ImageIOBase::CHAR:
    {
      if(!output_range_exists)
      {
        o_range[0]=itk::NumericTraits<char>::min();
        o_range[1]=itk::NumericTraits<char>::min();
      }
      dv::WindowImage<DIM, char>(IImage, OImage, scale, o_range);
      break;
    }

    case itk::ImageIOBase::USHORT:
    {
      if(!output_range_exists)
      {
        o_range[0]=itk::NumericTraits<unsigned short>::min();
        o_range[1]=itk::NumericTraits<unsigned short>::min();
      }
      dv::WindowImage<DIM, unsigned short>(IImage, OImage, scale, o_range);
      break;
    }

    case itk::ImageIOBase::SHORT:
    {
      if(!output_range_exists)
      {
        o_range[0]=itk::NumericTraits<short>::min();
        o_range[1]=itk::NumericTraits<short>::min();
      }
      dv::WindowImage<DIM, short>(IImage, OImage, scale, o_range);
      break;
    }

    case itk::ImageIOBase::UINT:
    {
      if(!output_range_exists)
      {
        o_range[0]=itk::NumericTraits<unsigned int>::min();
        o_range[1]=itk::NumericTraits<unsigned int>::min();
      }
      dv::WindowImage<DIM, unsigned int>(IImage, OImage, scale, o_range);
      break;
    }

    case itk::ImageIOBase::INT:
    {
      if(!output_range_exists)
      {
        o_range[0]=itk::NumericTraits<int>::min();
        o_range[1]=itk::NumericTraits<int>::min();
      }
      dv::WindowImage<DIM, int>(IImage, OImage, scale, o_range);
      break;
    }

    case itk::ImageIOBase::ULONG:
    {
      if(!output_range_exists)
      {
        o_range[0]=itk::NumericTraits<unsigned long>::min();
        o_range[1]=itk::NumericTraits<unsigned long>::min();
      }
      dv::WindowImage<DIM, unsigned long>(IImage, OImage, scale, o_range);
      break;
    }

    case itk::ImageIOBase::LONG:
    {
      if(!output_range_exists)
      {
        o_range[0]=itk::NumericTraits<long>::min();
        o_range[1]=itk::NumericTraits<long>::min();
      }
      dv::WindowImage<DIM, long>(IImage, OImage, scale, o_range);
      break;
    }

    case itk::ImageIOBase::FLOAT:
    {
      if(!output_range_exists)
      {
        o_range[0]=itk::NumericTraits<float>::min();
        o_range[1]=itk::NumericTraits<float>::min();
      }
      dv::WindowImage<DIM, float>(IImage, OImage, scale, o_range);
      break;
    }

    case itk::ImageIOBase::DOUBLE:
    {
      if(!output_range_exists)
      {
        o_range[0]=itk::NumericTraits<double>::min();
        o_range[1]=itk::NumericTraits<double>::min();
      }
      dv::WindowImage<DIM, double>(IImage, OImage, scale, o_range);
      break;
    }
    default:
    std::cerr << "ERROR: Unrecognized pixel type." << std::endl;
    return EXIT_FAILURE;
    break;
  }
  break;
}


return EXIT_SUCCESS;
}
