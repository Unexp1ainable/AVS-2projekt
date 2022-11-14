cmake_minimum_required(VERSION 3.21)

project(mandelbrot)

set(CMAKE_VERBOSE_MAKEFILE 1)
# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set(CMAKE_C_COMPILER "/opt/intel/oneapi/compiler/2022.2.0/linux/bin/icx")
# set(CMAKE_CXX_COMPILER "/opt/intel/oneapi/compiler/2022.2.0/linux/bin/icpx")

# set(CMAKE_PREFIX_PATH "/opt/intel/oneapi/compiler/2022.2.0/linux/IntelDPCPP")
# find_package(IntelDPCPP REQUIRED)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-fopenmp -O3 -mavx512f -mavx512dq -mavx512bw -mavx512vbmi -mavx512vbmi2 -mavx512vl")
  # using Clang
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # using GCC
    set(CMAKE_CXX_FLAGS "-O3 -march=native -mtune=native ${CMAKE_CXX_FLAGS}")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    # using icc
    set(CMAKE_CXX_FLAGS "-O3 -mavx2 -xHost -qopt-zmm-usage=high -g -qopenmp-simd -qopt-report=1 -qopt-report-phase=vec")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using Visual Studio C++
    set(CMAKE_CXX_FLAGS "/openmp /openmp:experimental /O2 /arch:AVX512")
endif()
# include(CMakePrintHelpers)
# set(CMAKE_CXX_FLAGS "-O3 -mavx2 -xHost -qopt-zmm-usage=high -g -qopenmp-simd -qopt-report=3 -qopt-report-phase=vec")
# cmake_print_variables(CMAKE_CXX_FLAGS)

find_package(ZLIB)
include_directories(${ZLIB_INCLUDE_DIRS})




set(SOURCE_FILES
    calculators/BaseMandelCalculator.cc
    calculators/BatchMandelCalculator.cc
    calculators/LineMandelCalculator.cc
    calculators/RefMandelCalculator.cc
    common/cnpy.cc
    main.cc
)

include_directories(common)
include_directories(calculators)

add_executable(mandelbrot ${SOURCE_FILES})
target_link_libraries(mandelbrot ${ZLIB_LIBRARIES})
