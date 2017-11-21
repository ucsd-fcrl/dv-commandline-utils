// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkMesh.h>
#include <itkImageFileReader.h>
#include <itkMeshFileWriter.h>
#include <itkSTLMeshIO.h>
#include <itkBinaryMask3DMeshSource.h>
#include <itkImage.h>
#include <set>
#include <itkUnaryFunctorImageFilter.h>

const unsigned int Dimension = 3;
typedef float      TCoordinate;

typedef unsigned char  PixelType;
typedef itk::Image< PixelType, Dimension >   ImageType;

typedef itk::Mesh<float,Dimension>                         MeshType;
typedef itk::BinaryMask3DMeshSource< ImageType, MeshType >   MeshSourceType;

typedef itk::Mesh< TCoordinate, Dimension > TMesh;
typedef itk::MeshFileWriter< TMesh >        TWriter;

namespace itk
{

  namespace Functor
  {

    template< class TInput, class TOutput>    class MyFilter 
    {
    public:
      MyFilter() {};    ~MyFilter() {};
      typedef typename NumericTraits<TInput>::RealType InputRealType;

      inline TOutput operator()( const TInput & A )
      {
        // Checks the functLabels for the value. Returns 1 if included. 0 otherwise
        return static_cast<TOutput>( functLabels.count(A) );
      }

      void SetLabels( std::set<PixelType> value )
      { functLabels = value; }

    private:
      std::set<PixelType>  functLabels;

    };
  };

  template<class TInputImage, class TOutputImage>
  class MyFunctorImageFilter :
  public UnaryFunctorImageFilter< TInputImage,TOutputImage,
  Functor::MyFilter< typename TInputImage::PixelType,typename TOutputImage::PixelType> >
  {
  public:


    typedef MyFunctorImageFilter          Self;
    typedef UnaryFunctorImageFilter< ImageType, ImageType,
    Functor::MyFilter< typename ImageType::PixelType,typename ImageType::PixelType > >  Superclass;

    typedef SmartPointer< Self >          Pointer;  typedef SmartPointer< const Self >    ConstPointer;

    itkNewMacro( Self );
    itkTypeMacro( MyFunctorImageFilter, UnaryFunctorImageFilter );

    typedef typename Superclass::FunctorType      FunctorType;
    typedef typename FunctorType::InputRealType   InputRealType;

    void SetLabels( std::set<PixelType> value )
    {    this->GetFunctor().SetLabels( value );    }

  protected:

    MyFunctorImageFilter() {}
    virtual ~MyFunctorImageFilter() {}


  private:

    MyFunctorImageFilter( const Self & ); // purposely not implemented
    void operator=(  const Self & );      // purposely not implemented

  };   // end of class
};


int main( int argc, char* argv[] )
{

  if ( 4 > argc )
  {
    std::cerr << "Usage:\n"<< std::endl;
    std::cerr << argv[0] << " <InputImage> <OutputMesh> <LabelValue1> <LabelValue2> .... <LabelValueN>" << std::endl;
    return EXIT_FAILURE;
  }

  const std::string inputFileName(argv[1]);
  const std::string outputFileName(argv[2]);

  //
  // Reader
  //

  typedef itk::ImageFileReader< ImageType >    ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( inputFileName );

  //
  // make Set from labels given
  //
  std::set<PixelType> labels;
  for (int i = 3; i < argc; ++i)
  {
    labels.insert( static_cast<PixelType>( std::atoi( argv[i] ) ) );
    std::cerr << "Label value: " << argv[i] << std::endl;
  }

  //
  // make unary filter which checks if pixels are in set
  //
  typedef itk::MyFunctorImageFilter<ImageType,ImageType >  FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetLabels( labels ); 
  filter->SetInput( reader->GetOutput() ); 

  //
  //Meshing
  //
  MeshSourceType::Pointer meshSource = MeshSourceType::New();
  const PixelType objectValue = static_cast<PixelType>( 1 );
  meshSource->SetObjectValue( objectValue );
  meshSource->SetInput( filter->GetOutput() );

  //
  // Writer
  //

  const auto writer = TWriter::New();
  writer->SetInput( meshSource->GetOutput() );
  writer->SetFileName( outputFileName );

  const auto o_ext = outputFileName.substr(outputFileName.size() - 3, 3);
  if (o_ext == "stl" || o_ext == "STL")
  {
    writer->SetMeshIO( itk::STLMeshIO::New() );
  }

  try
  {
    writer->Update();
  }
  catch ( itk::ExceptionObject & err )
  {
    std::cerr << "There was a problem writing the file." << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;

}
