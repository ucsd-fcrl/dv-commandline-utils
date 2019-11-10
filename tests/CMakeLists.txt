######################################
# Extra libraries needed for testing #
######################################

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

find_package(VTK REQUIRED)
if(${APPLE})
  find_package(Qt5 COMPONENTS
               Core
               Gui
               Widgets)
else()
  find_package(Qt5 COMPONENTS
               Core
               Gui
               Widgets
               X11Extras)
endif()

###########
## Tests ##
###########

set(EXNAMES "dvCameraStateTest"
            "dvStringOperationsTest"
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
            "dvSoftMaxTest"
            "dvDeleteIsolatedPointsTest"
            "dvMeshToQuadEdgeMeshTest"
   )

foreach(EXNAME ${EXNAMES})

  add_executable(${EXNAME} ${EXNAME}.cxx)
  target_link_libraries(${EXNAME} dv-commandline-utils-lib ${ITK_LIBRARIES} ${VTK_LIBRARIES})
  add_test(${EXNAME} ${EXNAME})

  vtk_module_autoinit(
    TARGETS ${EXNAME}
    MODULES ${VTK_LIBRARIES}
    )

endforeach()

#find_package(Ceres REQUIRED)
#include_directories(${CERES_INCLUDE_DIRS})
#
#add_executable(dvTriangleAspectRatioRegularizerTest dvTriangleAspectRatioRegularizerTest.cxx)
#target_link_libraries(dvTriangleAspectRatioRegularizerTest dv-commandline-utils-lib
#                                                           ${CERES_LIBRARIES}
#                                                           ${ITK_LIBRARIES})
#add_test(dvTriangleAspectRatioRegularizerTest dvTriangleAspectRatioRegularizerTest)

