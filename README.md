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

Spinning up a container to play around with the utilities can be as easy as:

```bash
$ docker run \
  --rm \
  -it \
  -v "${HOME}/hostdata":"/data" \
  -w /code/bin \
  sudomakeinstall/dv-commandline-utils zsh
(dkr) $ ./dv-add-mesh-noise --help
Allowed options:
  --help                Print usage information.
  --input-mesh arg      Filename of the input mesh.
  --output-mesh arg     Filename of the output image.
  --sigma arg           Amount of noise to be added.
(dkr) $ ./dv-add-mesh-noise \
  --input-mesh /data/input-mesh.obj \
  --output-mesh /data/output-mesh.obj \
  --sigma 0.1
```

Congrats, you're up and running!

# Developing

In order to set up for development, you'll want to mount your local copy of the source code on top of the copy shipped in the image, so that you can iteratively modify the code (on the local host machine) and build the binaries (in the container).  This just requires one further mount flat:


```bash
$ cd ~/Developer/repositories/dv-commandline-utils/src
$ docker run \
  --rm \
  -it \
  -v "${PWD}":/code/src" \
  -v "${HOME}/hostdata":"/data" \
  -w /code/bin \
  sudomakeinstall/dv-commandline-utils zsh
(dkr) $ # Modify and commit code on the host, build binaries here
```
