This repository contains a collection of commandline utilities for simple image and mesh manipulation, in order to aid batch processing.  The utilities are built on [ITK](https://itk.org/Doxygen/html/index.html).

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
$ ccmake ../src -DModule_IOSTL=ON
# Follow the instructions (c..c..g..)
```

If ITK has been installed in `~/Developer/ITK/bin`, the following instructions will work; otherwise, modify the `ITK_DIR` variable in `./src/CMakeLists.txt` to match the appropriate directory.

```
$ cd ~/Developer
$ git clone https://github.com/DVigneault/dv-commandline-utils.git
$ mkdir ./dv-commandline-utils/bin
$ cd ./dv-commandline-utils/bin
$ ccmake ../src
# Configure, configure, build...
```
