// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkImage.h>
#include <itkGDCMImageIO.h>
#include <itkImageSeriesReader.h>
#include <itkImageFileWriter.h>

// STD headers
#include <map>
#include <vector>
#include <string>

// GDCM headers
#include <gdcmReader.h>
#include <gdcmMediaStorage.h>
#include <gdcmStringFilter.h>
#include <gdcmAttribute.h>
#include <gdcmSorter.h>

// Custom
#include "includes/dvDICOMHeaderAttributes.h"

// Typedefs
typedef signed short PixelType;
const unsigned int Dimension = 3;
typedef itk::Image< PixelType, Dimension >  ImageType;
typedef itk::ImageSeriesReader< ImageType > ReaderType;
typedef itk::GDCMImageIO                    ImageIOType;
typedef itk::ImageFileWriter< ImageType > WriterType;

std::string
AddSlash(const char* arr)
{
  std::string str = arr;
  if ('/' != str.back()) str += '/';
  return str;
}

int
main( int argc, char* argv[] )
{

  /////////////////////////////////////
  // Deal with commandline arguments //
  /////////////////////////////////////

  if (3 < argc)
    {
    std::cerr << "Usage:\n"
              << argv[0] << " <inputDir> <outputDir>" << std::endl;
    return EXIT_FAILURE;
    }

  const std::string inputDirectory  = AddSlash(argv[1]);
  const std::string outputDirectory = AddSlash(argv[2]);
  const std::string outputExtension = (argc > 3) ? argv[3] : "mha";

  constexpr auto SliceAttributePair = std::make_pair(0x0020,0x0013);
  constexpr auto FrameAttributePair = std::make_pair(0x0008,0x0033);

  gdcm::Tag tSliceLocation(SliceAttributePair.first,SliceAttributePair.second);
  gdcm::Tag tFrameLocation(FrameAttributePair.first,FrameAttributePair.second);

  using TSliceAttribute = gdcm::Attribute<SliceAttributePair.first,SliceAttributePair.second>;
  using TFrameAttribute = gdcm::Attribute<FrameAttributePair.first,FrameAttributePair.second>;

  constexpr auto SliceSortFunction = &dv::SortByAttribute<TSliceAttribute>;
  constexpr auto FrameSortFunction = &dv::SortByAttribute<TFrameAttribute>;

  ///////////////////////////////////
  // Load and sort the input files //
  ///////////////////////////////////

  gdcm::Directory gdcmDir;
  gdcmDir.Load( inputDirectory.c_str() );

  std::cout << gdcmDir.GetFilenames().size() << " files were discovered." << std::endl;
  std::cout << "Sorting the images in the input directory..." << std::endl;

  // Define the tags we need
  std::set<gdcm::Tag> tags;
  tags.emplace(tSliceLocation);
  tags.emplace(tFrameLocation);

  gdcm::Sorter sorter;
  sorter.SetTagsToRead( tags );

  std::cout << "Sorting by frame number...";
  sorter.SetSortFunction( FrameSortFunction );
  sorter.StableSort( gdcmDir.GetFilenames() );
  std::cout << "done." << std::endl;

  std::cout << "Sorting by slice location...";
  sorter.SetSortFunction( SliceSortFunction );
  sorter.StableSort( sorter.GetFilenames() );
  std::cout << "done." << std::endl;

  // Get a sorted std::vector of file names
  const auto fileNames = sorter.GetFilenames();

  std::cout << "Number of names: " << fileNames.size() << std::endl;

  std::string previous = "";
  unsigned int frame = 0;

  std::cout << "Determining the frame number of each image..." << std::endl;

  // The frame is the key, the list of file names is the map
  std::map<unsigned int,std::vector<std::string>> frameMap;

  for (auto fileName = fileNames.begin();
       fileName != fileNames.end();
       ++fileName)
    {

    // Read in the file
    gdcm::Reader reader;
    reader.SetFileName( fileName->c_str() );

    if (!reader.ReadSelectedTags(tags))
      {
      std::cerr << "Couldn't read the selected tags." << std::endl;
      return EXIT_FAILURE;
      }

    for (const auto t : tags)
      {
      if (!reader.GetFile().GetDataSet().FindDataElement( t ))
        {
        std::cerr << "Couldn't find the tag: " << t << std::endl;
        return EXIT_FAILURE;
        }
      }

    // Get the values of the tags
    gdcm::StringFilter filter;
    filter.SetFile( reader.GetFile() );

    const std::string sl = filter.ToString( tSliceLocation );

    // Calculate the frame number
    (sl == previous) ? frame++ : frame = 0;
    previous = sl;

    // Add the file name to the relevant list
    auto &v = frameMap[frame];
    v.push_back(*fileName);

    }

  // Print out some info
  std::cout << "Number of frames: " << frameMap.size() << std::endl;

  // Make sure each frame has the same number of slices
  for (unsigned int i = 0; i + 1 < frameMap.size(); ++i)
    {
    if (frameMap[i].size() != frameMap[i+1].size())
      {
      std::cerr << "Error: Different number of slices!" << std::endl;
      std::cerr << "Frame " << i << ": " << frameMap[i].size() << " slices." << std::endl;
      std::cerr << "Frame " << (i+1) << ": " << frameMap[i+1].size() << " slices." << std::endl;
      return EXIT_FAILURE;
      }
    }

  for (auto it = frameMap.cbegin(); it != frameMap.cend(); ++it)
    {

    ReaderType::Pointer reader = ReaderType::New();
    ImageIOType::Pointer dicomIO = ImageIOType::New();
    reader->SetImageIO( dicomIO );
    reader->SetFileNames( it->second );

    try
      {
      reader->Update();
      }
    catch (itk::ExceptionObject &ex)
      {
      std::cout << ex << std::endl;
      return EXIT_FAILURE;
      }

    const std::string outputFileName
      = outputDirectory + std::to_string(it->first) + ".vtk";
    std::cout << "Writing File: " << outputFileName << std::endl;

    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName( outputFileName );
    writer->SetInput( reader->GetOutput() );
  
    try
      {
      writer->Update();
      }
    catch (itk::ExceptionObject &ex)
      {
      std::cout << ex << std::endl;
      return EXIT_FAILURE;
      }

    }

  return EXIT_SUCCESS;

}

