project (pitch_detection)
cmake_minimum_required (VERSION 3.1)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi -pedantic -Werror -Wall -O3 -std=c++17")

set (SOURCE_DIR "./src")
set (SOURCE_FILES "./main.cpp;./src/yin.hpp;./src/mpm.hpp;./src/autocorrelation.hpp;./src/pitch.hpp;./parabolic_interpolation/parabolic_interpolation.hpp;./sinewave/sinewave.hpp;./autocovariance/autocovariance.hpp")
set (FFMPEG_FILES "./src/file.hpp")

include_directories ("${SOURCE_DIR}")
include_directories ("./parabolic_interpolation")
include_directories ("./sinewave")
include_directories ("./autocovariance")

set (LIBS "gflags")

# find third party modules
set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)
find_package (gflags REQUIRED)
find_package (Ffmpeg)
find_package (FFTW)
# end find third party modules

if (NOT GFLAGS_FOUND)
  MESSAGE(FATAL_ERROR "gflags required")
endif ()

if (FFMPEG_FOUND)
  set (SOURCE_FILES "${SOURCE_FILES};${FFMPEG_FILES}")
  LIST (APPEND LIBS "avcodec" "avformat" "avutil")
  include_directories (${FFMPEG_DIR})
  add_definitions (-DFFMPEG_ENABLED)
endif (FFMPEG_FOUND)

if (FFTW_FOUND)
  LIST (APPEND LIBS "fftw3")
  add_definitions (-DFFTW_ENABLED)
endif (FFTW_FOUND)

add_executable (pitch_detection ${SOURCE_FILES})
target_link_libraries (pitch_detection ${LIBS})
