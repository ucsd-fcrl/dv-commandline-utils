This repository contains a collection of commandline utilities for simple image and mesh manipulation, in order to aid batch processing.  The utilities are built on [ITK](https://itk.org/Doxygen/html/index.html).  The scope of each utility is similar to the various ITK examples that come with the repository, with the following important changes:

- Commandline options are parsed using the `boost::program_options` library.
- The pixel type is determined from the pixel type of the input file, and the appropriate template instantiation is determined dynamically.
- The output is written to disk, as opposed to the many ITK examples with use the `QuickView` utility.

Currently, only volumetric data are supported; 2D support is forthcoming.

# Prerequisites

## Mac OsX

These instructions assume that you have MacPorts installed.

```bash
# Install prerequisits using MacPorts:
$ sudo port install tiff boost cmake eigen3
```

## Ubuntu

```bash
# Install prerequisites using aptitude:
$ sudo apt install libtiff5-dev libboost-all-dev libeigen3-dev cmake cmake-curses-gui
# CMake must be installed separately, because the aptitude version is out of date.
$ mkdir ~/Developer/cmake && cd ~/Developer/cmake/
$ git clone https://github.com/Kitware/CMake.git src
$ mkdir ./bin && cd ./bin
$ ccmake ../src
# Follow the instructions (c..c..g..)
$ sudo make install
```

# Instructions for Building

```bash
# Install FGT:
$ mkdir ~/Developer/fgt/ && cd ~/Developer/fgt/
$ git clone https://github.com/gadomski/fgt.git src
$ mkdir ./bin && cd ./bin
$ ccmake ../src -DCMAKE_CXX_STANDARD=14 -DWITH_TESTS=OFF
# Follow the instructions (c..c..g..)
$ make && sudo make install

# Install CPD:
$ mkdir ~/Developer/cpd/ && cd ~/Developer/cpd/
$ git clone https://github.com/gadomski/cpd.git src
$ mkdir ./bin && cd ./bin
$ ccmake ../src -DCMAKE_CXX_STANDARD=14 -DWITH_TESTS=OFF -DWITH_FGT=ON
# Follow the instructions (c..c..g..)
$ make && sudo make install

# Build VTK:
$ mkdir ~/Developer/VTK && cd ~/Developer/VTK
$ git clone https://github.com/Kitware/VTK.git src
$ mkdir ./bin && cd ./bin
$ ccmake ../src \
  -DCMAKE_CXX_STANDARD=14 \
  -DBUILD_TESTING=OFF \
  -DBUILD_EXAMPLES=OFF \
  -DBUILD_SHARED_LIBS=OFF \
  -DVTK_USE_SYSTEM_TIFF=ON
# Follow the instructions (c..c..g..)
$ make

# Build ITK:
$ mkdir ~/Developer/ITK && cd ~/Developer/ITK/
$ git clone https://github.com/InsightSoftwareConsortium/ITK.git src
$ mkdir ./bin && cd ./bin
$ ccmake ../src \
  -DCMAKE_CXX_STANDARD=14 \
  -DBUILD_TESTING=OFF \
  -DBUILD_EXAMPLES=OFF \
  -DBUILD_SHARED_LIBS=OFF \
  -DITK_USE_SYSTEM_TIFF=ON \
  -DModule_DVMeshNoise=ON \
  -DModule_IOSTL=ON \
  -DModule_SubdivisionQuadEdgeMesh=ON
# Follow the instructions (c..c..g..)
$ make

# Build this repo:
$ cd ~/Developer/
$ git clone https://github.com/DVigneault/dv-commandline-utils.git
$ mkdir -p ./dv-commandline-utils/bin && cd ./dv-commandline-utils/bin
$ ccmake ../src \
  -DCAMKE_CXX_STANDARD=14 \
  -DBUILD_CPD_UTILS=ON \
  -DBUILD_VTK_UTILS=ON
# Follow the instructions (c..c..g..)
$ make
```
