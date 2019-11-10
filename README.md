# `dv-commandline-utils`

This repository contains a collection of commandline utilities for simple image and mesh manipulation, in order to aid batch processing.  The utilities are built on [ITK](https://itk.org/Doxygen/html/index.html).  The scope of each utility is similar to the various ITK examples that come with the repository, with the following important changes:

- Commandline options are parsed using the `boost::program_options` library.
- Ideally, the pixel type is determined from the pixel type of the input file, and the appropriate template instantiation is determined dynamically.
- The output is written to disk, as opposed to the many ITK examples which use the `QuickView` utility.

# Building

The preferred mechanism for building this repository is using [Docker](https://www.docker.com/).  Please make sure that Docker is installed and running on your system.  At that point, the repository can be built as follows:

```bash
$ git clone https://github.com/DVigneault/dv-commandline-utils.git \
    ~/Developer/repositories/dv-commandline-utils/src
$ cd ~/Developer/repositories/dv-commandline-utils/src
$ ./docker-build.sh
```

`dv-commandline-utils` depends on quite a few packages, and two large c++ libraries (ITK and VTK), which must be built from source--so this command could take a while to finish.  Once it finishes, you should be able to see `sudomakeinstall/dv-commandline-utils` in the list of docker images on your computer:

```bash
$ docker image ls
REPOSITORY                             TAG                 IMAGE ID            CREATED             SIZE
sudomakeinstall/dv-commandline-utils   latest              848b802899d4        43 seconds ago      3.08GB
```

# Running

Spinning up a container to play around with the utilities can be as easy as:

```bash
$ ./docker-run.sh
(dkr) $ ./dv-add-mesh-noise --help
Allowed options:
  --help                Print usage information.
  --input-mesh arg      Filename of the input mesh.
  --output-mesh arg     Filename of the output image.
  --sigma arg           Amount of noise to be added.
```

Congrats, you're up and running!  However, in order to access data on your host machine, or build the local copy of the code, you'll want to mount some volumes, similar to docker-compose.override.yml.example.  Copy this over and edit the paths as necessary to get started:

```bash
$ cp ./docker-compose.override.yml.example ./docker-compose.override.yml
$ vim ./docker-compose.override.yml # Edit paths; this file will be ignored by git.
$ ./docker-run.sh
```

# Running Tests

Unit tests are not automatically built in the docker image.  The steps to build and run tests are as follows:

```bash
$ cd ~/Developer/repositories/dv-commandline-utils/src
$ ./docker-run.sh
(dkr) $ ccmake ../src -DBUILD_TESTING=ON
# configure, configure, generate in the curses GUI.
(dkr) $ make -j$(nproc)
(dkr) $ ctest
```

You can also make and run one specific test:

```
(dkr) $ make dvCameraStateTest
# V for Verboase, R for Regex
(dkr) $ ctest -V -R dvCameraState
```
