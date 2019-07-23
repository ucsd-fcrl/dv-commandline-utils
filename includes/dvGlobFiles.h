#ifndef dvGlobFiles_h
#define dvGlobFiles_h

#include <glob.h>
#include <vector>
#include <string>

namespace dv
{

std::vector<std::string>
GlobFiles( const std::string &pattern )
{

  std::vector<std::string> output;

  //Declare glob_t for storing the results of globbing
  glob_t globbuf;
  
  //Glob.. GLOB_TILDE tells the globber to expand "~" in the pattern to the home directory
  glob( pattern.c_str(), GLOB_TILDE, NULL, &globbuf);

  for ( size_t i = 0; i < globbuf.gl_pathc; ++i )
    {
    output.push_back( globbuf.gl_pathv[i] );
    }
  
  //Free the globbuf structure
  if ( globbuf.gl_pathc > 0 )
    {
    globfree( &globbuf );
    }

  return output;
  
}

}

#endif
