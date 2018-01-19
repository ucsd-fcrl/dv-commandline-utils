This repository contains a collection of commandline utilities for simple image and mesh manipulation, in order to aid batch processing.  The utilities are built on [ITK](https://itk.org/Doxygen/html/index.html).  The scope of each utility is similar to the various ITK examples that come with the repository, with the following important changes:

- Commandline options are parsed using the `boost::program_options` library.
- The pixel type is determined from the pixel type of the input file, and the appropriate template instantiation is determined dynamically.
- The output is written to disk, as opposed to the many ITK examples with use the `QuickView` utility.

Currently, only volumetric data are supported; 2D support is forthcoming.

# Instructions for Building (Mac Os X)

These instructions assume that you have MacPorts installed.

```
NPROC=$(sysctl -n hw.ncpu)
# Install prerequisits using MacPorts:
$ sudo port install tiff boost cmake eigen3

# Install FGT:
$ mkdir ~/Developer/fgt/ && cd ~/Developer/fgt/
$ git clone https://github.com/gadomski/fgt.git src
$ mkdir ./bin && cd ./bin
$ ccmake ../src -DCMAKE_CXX_FLAGS=-std=c++14 -DWITH_TESTS=OFF
# Follow the instructions (c..c..g..)
$ make -j${NPROC} && sudo make install

# Install CPD:
$ mkdir ~/Developer/cpd/ && cd ~/Developer/cpd/
$ git clone https://github.com/gadomski/cpd.git src
$ mkdir ./bin && cd ./bin
$ ccmake ../src -DCMAKE_CXX_FLAGS=-std=c++14 -DWITH_TESTS=OFF -DWITH_FGT=ON
# Follow the instructions (c..c..g..)
$ make -j${NPROC} && sudo make install

# Build ITK:
$ mkdir ~/Developer/ITK && cd ~/Developer/ITK/
$ git clone https://github.com/InsightSoftwareConsortium/ITK.git src
$ mkdir ./bin && cd ./bin
$ ccmake ../src -DCMAKE_CXX_FLAGS=-std=c++14 -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF -DITK_USE_SYSTEM_TIFF=ON -DModule_IOSTL=ON -DModule_SubdivisionQuadEdgeMesh=ON
# Follow the instructions (c..c..g..)
$ make -j${NPROC}

# Build this repo:
$ cd ~/Developer/
$ git clone https://github.com/DVigneault/dv-commandline-utils.git
$ mkdir -p ./dv-commandline-utils/bin && cd ./dv-commandline-utils/bin
$ ccmake ../src -DCAMKE_CXX_FLAGS=-std=c++14
# Follow the instructions (c..c..g..)
$ make -j${NPROC}
```

# Instructions for Building

These instructions assume that you are on a unix system and that the CMake Curses Gui is installed.

First, make sure that ITK is built on your system.

```
# Make and move into a directory where source and build files for ITK will live
$ mkdir ~/Developer/ITK/ && cd ~/Developer/ITK/

# Clone the ITK git repo
$ git clone https://github.com/InsightSoftwareConsortium/ITK.git ./src

# Make and move into the binary directory
$ mkdir ./bin && cd ./bin

# Configure and generate
$ ccmake ../src -DModule_IOSTL=ON -DCMAKE_CXX_FLAGS=-std=c++14
# Follow the instructions (c..c..g..)

# Build ITK
$ make -j$(nproc)
```

If ITK has been installed in `~/Developer/ITK/bin`, the following instructions will work; otherwise, modify the `ITK_DIR` variable in `./src/CMakeLists.txt` to match the appropriate directory.

```
$ cd ~/Developer
$ git clone https://github.com/DVigneault/dv-commandline-utils.git
$ mkdir ./dv-commandline-utils/bin
$ cd ./dv-commandline-utils/bin
$ ccmake ../src
# Configure, configure, build...
$ make -j$(nproc)
```
