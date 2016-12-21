project(pitch_detection)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi -pedantic -Werror -Wall -O3 -std=c++17")
set(SOURCE_DIR "./src")
set(SOURCE_FILES "")
set(AUTOCORR_DIR "./src/autocorrelation")
set(SPECTRUM_DIR "./src/spectrum")

include_directories ("./parabolic_interpolation")
set(SOURCE_FILES "${SOURCE_FILES};./parabolic_interpolation/parabolic_interpolation.hpp")

include_directories ("./sinewave")
set(SOURCE_FILES "${SOURCE_FILES};./sinewave/sinewave.hpp")

include_directories ("./autocovariance")
set(SOURCE_FILES "${SOURCE_FILES};./autocovariance/autocovariance.hpp")

set(SOURCE_FILES_AUTOCORR "./main.cpp;./src/autocorrelation/yin.hpp;./src/autocorrelation/mpm.hpp;")
set(SOURCE_FILES_SPECTRUM "./src/spectrum/loop.hpp;./src/spectrum/goertzel.hpp;./src/spectrum/dft.hpp")
set(SOURCE_FILES "${SOURCE_FILES};${SOURCE_FILES_AUTOCORR};${SOURCE_FILES_SPECTRUM};./src/pitch.hpp;")
set(FFMPEG_FILES "./src/file.hpp")
set(LIBS "gflags")

include_directories("${SOURCE_DIR}")
include_directories("${AUTOCORR_DIR}")
include_directories("${SPECTRUM_DIR}")

# find third party modules
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)
find_package(gflags REQUIRED)
find_package(Ffmpeg)
find_package(FFTW)
# end find third party modules

if (NOT GFLAGS_FOUND)
  MESSAGE(FATAL_ERROR "gflags required")
endif ()

if (FFMPEG_FOUND)
  set(SOURCE_FILES "${SOURCE_FILES};${FFMPEG_FILES}")
  LIST(APPEND LIBS "avcodec" "avformat" "avutil")
  include_directories(${FFMPEG_DIR})
  add_definitions(-DFFMPEG_ENABLED)
endif (FFMPEG_FOUND)

if (FFTW_FOUND)
  LIST(APPEND LIBS "fftw3")
  add_definitions(-DFFTW_ENABLED)
endif (FFTW_FOUND)

add_executable(pitch_detection ${SOURCE_FILES})
target_link_libraries(pitch_detection ${LIBS})
