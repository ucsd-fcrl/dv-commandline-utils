cmake_minimum_required(VERSION 2.8.12)

project(dv-image-information)

set(CMAKE_CXX_FLAGS "-std=c++14 -fopenmp" CACHE STRING "cxx compiler flags" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -pg" CACHE STRING "cxx debug compiler flags" FORCE)
#set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "build type" FORCE)

find_package(OpenMP REQUIRED)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

find_package(Cpd REQUIRED)
find_package(ITK REQUIRED)

find_package(Boost COMPONENTS program_options filesystem system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB SRC src/*.cxx)

include_directories("includes/")
add_library(dv-lib ${SRC})

set(EXNAME "dv-convert-dicom-series-to-volume")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-convert-mesh")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-convert-image")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-image-information")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-resample-volume")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-resample-from-reference")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-scale-mesh")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-translate-mesh")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-binarize-mesh")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-collapse-4d")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-permute-axes-3d")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-flip-along-axes-3d")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-change-information-3d")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-binary-threshold")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-dicom-information")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-convert-labels-to-mesh")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-decimate-mesh")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-extract-connected-components")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-fill-holes-in-binary-image")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-binary-opening")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-segmentation-error")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-subdivide-mesh-loop")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-add-mesh-noise")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-mesh-similarity-vertex-distance")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-mesh-area-volume")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-extract-interface-between-segments")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-smooth-mesh")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(EXNAME "dv-nonrigid-cpd")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} dv-lib
                                Cpd::Library-C++
                                ${ITK_LIBRARIES}
                                ${Boost_PROGRAM_OPTIONS_LIBRARY}
                                ${Boost_FILESYSTEM_LIBRARY}
                                ${Boost_SYSTEM_LIBRARY}
                                ${OpenMP_LIBRARIES})

find_package(VTK)

if(VTK_FOUND)
  include(${VTK_USE_FILE})
  find_package(Qt5 REQUIRED
               Core
               Gui
               Widgets
               X11Extras)

  set(EXNAME "dv-mesh-quality")
  add_executable(${EXNAME} ${EXNAME}.cxx)
  target_link_libraries(${EXNAME} ${VTK_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})

endif()

