cmake_minimum_required(VERSION 2.8.12)
 
project(dv-image-information)

set(ITK_DIR "~/Developer/ITK/bin")
set(CMAKE_CXX_FLAGS "-std=c++11" CACHE STRING "cxx compiler flags" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -Werror" CACHE STRING "cxx debug compiler flags" FORCE)
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "build type" FORCE)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

set(EXNAME "dv-convert-dicom-series-to-volume")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES})

set(EXNAME "dv-convert-mesh")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES})

set(EXNAME "dv-image-information")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES})

set(EXNAME "dv-resample-volume")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES})

set(EXNAME "dv-scale-mesh")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES})

set(EXNAME "dv-translate-mesh")
add_executable(${EXNAME} ${EXNAME}.cxx)
target_link_libraries(${EXNAME} ${ITK_LIBRARIES})

