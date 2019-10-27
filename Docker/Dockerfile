# syntax=docker/dockerfile:experimental
FROM ubuntu:19.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
  && apt-get install -y --no-install-recommends \
  openssh-client \
  ca-certificates \
  clang \
  clang-tidy \
  clang-format \
  git \
  vim \
  libeigen3-dev \
  cmake \
  cmake-curses-gui \
  coreutils \
  freeglut3-dev \
  qt5-default \
  qt5-qmake \
  libqt5x11extras5-dev \
  rapidjson-dev \
  libboost-filesystem-dev \
  libboost-program-options-dev \
  zsh \
  curl \
  && rm -rf /var/lib/apt/lists/*

RUN sh -c "$(curl -fsSL https://raw.githubusercontent.com/robbyrussell/oh-my-zsh/master/tools/install.sh)"

# Build VTK
RUN mkdir -p /Developer/VTK/bin \
  && cd /Developer/VTK \
  && git clone --depth 1 https://github.com/Kitware/VTK.git src \
  && cd /Developer/VTK/bin \
  && cmake ../src \
    -DCMAKE_CXX_STANDARD=14 \
    -DCMAKE_CXX_FLAGS=-std=c++14 \
    -DVTK_BUILD_TESTING=OFF \
    -DVTK_BUILD_EXAMPLES=OFF \
    -DBUILD_SHARED_LIBS=OFF \
    -DVTK_GROUP_ENABLE_Qt=YES \
  && make -j$(nproc) \
  && make install \
  && cd /Developer \
  && rm -rf ./VTK

# Build ITK
RUN mkdir -p /Developer/ITK/bin \
  && cd /Developer/ITK \
  && git clone --depth 1 https://github.com/InsightSoftwareConsortium/ITK.git src \
  && cd /Developer/ITK/bin \
  && cmake ../src \
    -DCMAKE_CXX_STANDARD=14 \
    -DCMAKE_CXX_FLAGS=-std=c++14 \
    -DBUILD_TESTING=OFF \
    -DBUILD_EXAMPLES=OFF \
    -DBUILD_SHARED_LIBS=OFF \
    -DModule_ITKReview=ON \
    -DModule_MeshNoise=ON \
    -DModule_IOMeshSTL=ON \
    -DModule_SubdivisionQuadEdgeMeshFilter=ON \
    -DModule_ITKVtkGlue=ON \
  && make -j$(nproc) \
  && make install \
  && cd /Developer \
  && rm -rf ./ITK

RUN mkdir -p /Developer/repositories/dv-commandline-utils/bin \
  && cd /Developer/repositories/dv-commandline-utils \
  && git clone --depth 1 https://github.com/dvigneault/dv-commandline-utils.git src \
  && cd ./bin \
  && cmake ../src  \
    -DCMAKE_CXX_STANDARD=14 \
    -DCMAKE_CXX_FLAGS=-std=c++14 \
  && make -j$(nproc)

### Download public key for github.com
##RUN mkdir -p -m 0600 ~/.ssh && ssh-keyscan github.com >> ~/.ssh/known_hosts
##
### Using ssh-agent forwarding to clone from private git repo without copying private keys into the image
### https://docs.docker.com/develop/develop-images/build_enhancements/
### https://blog.oddbit.com/post/2019-02-24-docker-build-learns-about-secr/
### https://medium.com/@tonistiigi/build-secrets-and-ssh-forwarding-in-docker-18-09-ae8161d066
### https://developer.github.com/v3/guides/using-ssh-agent-forwarding/
##RUN --mount=type=ssh git clone git@github.com:dvigneault/resume.git Developer/resume
