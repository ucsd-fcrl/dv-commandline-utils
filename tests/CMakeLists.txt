######################################
# Extra libraries needed for testing #
######################################

find_package(Ceres REQUIRED)
include_directories(${CERES_INCLUDE_DIRS})

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

###########
## Tests ##
###########

set(EXNAMES "dvStringOperationsTest"
            "dvVectorOperationsTest"
            "dvMathTest"
            "dvProgressTest"
            "dvJoinTest"
            "dvCycleTest"
            "dvGlobFilesTest"
            "dvCyclicMeanTest"
            "dvXORSwapTest"
            "dvSortTest"
            "dvHeapTest"
            "dvReverseFindIfTest"
            "dvSmartPointerTest"
            "dvSoftMaxTest"
            "dvDeleteIsolatedPointsTest"
            "dvMeshToQuadEdgeMeshTest"
   )

foreach(EXNAME ${EXNAMES})

  add_executable(${EXNAME} ${EXNAME}.cxx)
  target_link_libraries(${EXNAME} dv-commandline-utils-lib ${ITK_LIBRARIES})
  add_test(${EXNAME} ${EXNAME})

endforeach()


add_executable(dvTriangleAspectRatioRegularizerTest dvTriangleAspectRatioRegularizerTest.cxx)
target_link_libraries(dvTriangleAspectRatioRegularizerTest dv-commandline-utils-lib
                                                           ${CERES_LIBRARIES}
                                                           ${ITK_LIBRARIES})
add_test(dvTriangleAspectRatioRegularizerTest dvTriangleAspectRatioRegularizerTest)

