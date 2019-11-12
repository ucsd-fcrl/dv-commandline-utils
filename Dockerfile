FROM ubuntu:19.04

ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
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

# Build Boost
RUN mkdir -p /Developer/boost \
  && cd /Developer/boost \
  && curl -L https://dl.bintray.com/boostorg/release/1.71.0/source/boost_1_71_0.tar.gz -o ./boost.tar.gz \
  && tar -xvzf ./boost.tar.gz -C ./ --strip-components=1 \
  && ./bootstrap.sh \
  && ./b2 link=static --with-program_options --with-filesystem install --with-system \
  && cd /Developer \
  && rm -rf ./boost

# Add the source for this repository
ADD . /code/src/

# Build this repo
RUN mkdir -p /code/bin \
  && cd /code/bin \
  && cmake ../src  \
    -DCMAKE_CXX_STANDARD=14 \
    -DCMAKE_CXX_FLAGS=-std=c++14 \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DBoost_USE_STATIC_LIBS=ON \
  && make -j$(nproc)
