project(pitch_detection)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi -pedantic -Werror -Wall -O3 -std=c++17")
set(SOURCE_DIR "./src")
set(AUTOCORR_DIR "./src/autocorrelation")
set(SPECTRUM_DIR "./src/spectrum")
set(SOURCE_FILES_AUTOCORR "./main.cpp;./src/autocorrelation/yin.cpp;./src/autocorrelation/yin.hpp;./src/autocorrelation/mpm.cpp;./src/autocorrelation/mpm.hpp;./src/autocorrelation/mpm_yin_common.cpp;./src/autocorrelation/mpm_yin_common.hpp")
set(SOURCE_FILES_SPECTRUM "./src/spectrum/loop.cpp;./src/spectrum/loop.hpp;./src/spectrum/goertzel.cpp;./src/spectrum/goertzel.hpp;./src/spectrum/dft.cpp;./src/spectrum/dft.hpp")
set(SOURCE_FILES "${SOURCE_FILES_AUTOCORR};${SOURCE_FILES_SPECTRUM};./src/sine.cpp;./src/sine.hpp;./src/pitch.cpp;./src/pitch.hpp;./src/helper.cpp;./src/helper.hpp;")
set(FFMPEG_FILES "./src/file.cpp;./src/file.hpp")
set(FFTW_FILES "./src/autocorrelation/autocorrelation.cpp;./src/autocorrelation/autocorrelation.hpp")
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
  set(SOURCE_FILES "${SOURCE_FILES};${FFTW_FILES}")
  LIST(APPEND LIBS "fftw3")
  include_directories(${FFTW_DIR})
  add_definitions(-DFFTW_ENABLED)
endif (FFTW_FOUND)

add_executable(pitch_detection ${SOURCE_FILES})
target_link_libraries(pitch_detection ${LIBS})
