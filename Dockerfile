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

ADD . /code/src/

RUN mkdir -p /code/bin \
  && cd /code/bin \
  && cmake ../src  \
    -DCMAKE_CXX_STANDARD=14 \
    -DCMAKE_CXX_FLAGS=-std=c++14 \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  && make -j$(nproc)
