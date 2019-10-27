This repository contains a collection of commandline utilities for simple image and mesh manipulation, in order to aid batch processing.  The utilities are built on [ITK](https://itk.org/Doxygen/html/index.html).  The scope of each utility is similar to the various ITK examples that come with the repository, with the following important changes:

- Commandline options are parsed using the `boost::program_options` library.
- Ideally, the pixel type is determined from the pixel type of the input file, and the appropriate template instantiation is determined dynamically.
- The output is written to disk, as opposed to the many ITK examples which use the `QuickView` utility.

Currently, only volumetric data are supported.

# Building

The preferred mechanism for building this repository is using [Docker](https://www.docker.com/).  Please make sure that Docker is installed and running on your system.  At that point, the repository can be built as follows:

```bash
$ mkdir -p ~/Developer/dv-commandline-utils
$ cd ~/Developer/dv-commandline-utils
$ git clone https://github.com/DVigneault/dv-commandline-utils.git src
$ cd ./src
$ ./docker-build.sh
```

# Developing

The docker image can be spun up into a container using:

```bash
$ docker run --rm -it sudomakeinstall/dv-commandline-utils zsh
(docker) $
```

However, as Docker containers are ephemeral, it is not advisable to edit the source code within the container directly.  Rather, the `-v` flat is used to mount the host copy of the code (`clone`d into `~/Developer/dv-commandline-utils/src` above) onto `/Developer/dv-commandline-utils/src` in the container.  This way, the source code may be edited (either on the host or the container) and rebuilt iteratively in the container.  This way, source code changes will persist on the host machine after the container exits.

```bash
$ cd ~/Developer/dv-commandline-utils/src
$ ./docker-ruh.sh
# edit source here, re-build in ../bin
```
