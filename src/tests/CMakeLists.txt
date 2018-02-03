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

add_executable(dvStringOperationsTest dvStringOperationsTest.cxx)
target_link_libraries(dvStringOperationsTest dv-commandline-utils-lib)
add_test(dvStringOperationsTest dvStringOperationsTest)

add_executable(dvVectorOperationsTest dvVectorOperationsTest.cxx)
target_link_libraries(dvVectorOperationsTest dv-commandline-utils-lib)
add_test(dvVectorOperationsTest dvVectorOperationsTest)

add_executable(dvMathTest dvMathTest.cxx)
target_link_libraries(dvMathTest dv-commandline-utils-lib)
add_test(dvMathTest dvMathTest)

add_executable(dvProgressTest dvProgressTest.cxx)
target_link_libraries(dvProgressTest dv-commandline-utils-lib)
add_test(dvProgressTest dvProgressTest)

add_executable(dvJoinTest dvJoinTest.cxx)
target_link_libraries(dvJoinTest dv-commandline-utils-lib)
add_test(dvJoinTest dvJoinTest)

add_executable(dvCycleTest dvCycleTest.cxx)
target_link_libraries(dvCycleTest dv-commandline-utils-lib)
add_test(dvCycleTest dvCycleTest)

add_executable(dvGlobFilesTest dvGlobFilesTest.cxx)
target_link_libraries(dvGlobFilesTest dv-commandline-utils-lib)
add_test(dvGlobFilesTest dvGlobFilesTest)

#add_executable(dvCyclicMeanTest dvCyclicMeanTest.cxx)
#target_link_libraries(dvCyclicMeanTest dv-commandline-utils-lib)
#add_test(dvCyclicMeanTest dvCyclicMeanTest)

add_executable(dvXORSwapTest dvXORSwapTest.cxx)
target_link_libraries(dvXORSwapTest dv-commandline-utils-lib)
add_test(dvXORSwapTest dvXORSwapTest)

add_executable(dvSortTest dvSortTest.cxx)
target_link_libraries(dvSortTest dv-commandline-utils-lib)
add_test(dvSortTest dvSortTest)

add_executable(dvHeapTest dvHeapTest.cxx)
target_link_libraries(dvHeapTest dv-commandline-utils-lib)
add_test(dvHeapTest dvHeapTest)

add_executable(dvReverseFindIfTest dvReverseFindIfTest.cxx)
target_link_libraries(dvReverseFindIfTest dv-commandline-utils-lib)
add_test(dvReverseFindIfTest dvReverseFindIfTest)

add_executable(dvSmartPointerTest dvSmartPointerTest.cxx)
target_link_libraries(dvSmartPointerTest dv-commandline-utils-lib)
add_test(dvSmartPointerTest dvSmartPointerTest)

add_executable(dvSoftMaxTest dvSoftMaxTest.cxx)
target_link_libraries(dvSoftMaxTest dv-commandline-utils-lib)
add_test(dvSoftMaxTest dvSoftMaxTest)

add_executable(dvTriangleAspectRatioRegularizerTest dvTriangleAspectRatioRegularizerTest.cxx)
target_link_libraries(dvTriangleAspectRatioRegularizerTest dv-commandline-utils-lib
                                                           ${CERES_LIBRARIES}
                                                           ${ITK_LIBRARIES})
add_test(dvTriangleAspectRatioRegularizerTest dvTriangleAspectRatioRegularizerTest)

