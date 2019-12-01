######################################
# Extra libraries needed for testing #
######################################

find_package(Boost COMPONENTS program_options filesystem system REQUIRED)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

find_package(Ceres REQUIRED)
include_directories(${CERES_INCLUDE_DIRS})

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

set(EXNAMES "dvAccelerationRegularizerTest"
            "dvCameraStateTest"
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
            "dvVisualizeMeshesInteractorStyleTest"
            "dvVisualizeMeshesFrameTest"
            "dvMeshViewTest"
            "dvTextViewTest"
            "dvSegmentationViewTest"
   )

foreach(EXNAME ${EXNAMES})

  add_executable(${EXNAME} src/${EXNAME}.cxx)
  target_link_libraries(${EXNAME} dv-commandline-utils-lib ${ITK_LIBRARIES} ${VTK_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY})
  add_test(${EXNAME} ${EXNAME})

  vtk_module_autoinit(
    TARGETS ${EXNAME}
    MODULES ${VTK_LIBRARIES}
    )

endforeach()
