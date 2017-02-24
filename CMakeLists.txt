project (pitch_detection)
cmake_minimum_required (VERSION 3.1)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi -pedantic -Werror -Wall -O3 -std=c++17")

set (SOURCE_DIR "./src")
set (SOURCE_FILES "./src/main.cpp;./src/yin.hpp;./src/mpm.hpp;./src/autocorrelation.hpp;./src/parabolic_interpolation.hpp;./src/sinewave.hpp;./src/types.hpp")
set (FFMPEG_FILES "./src/file.hpp")

include_directories ("${SOURCE_DIR}")

# find third party modules
set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)
find_package (gflags REQUIRED)
find_package (xcorr REQUIRED)
find_package (Ffmpeg)
# end find third party modules

if (NOT GFLAGS_FOUND)
  MESSAGE(FATAL_ERROR "gflags required")
endif ()

if (NOT XCORR_FOUND)
  MESSAGE(FATAL_ERROR "xcorr required")
endif ()

set (LIBS "gflags -lxcorr")

if (FFMPEG_FOUND)
  set (SOURCE_FILES "${SOURCE_FILES};${FFMPEG_FILES}")
  LIST (APPEND LIBS "avcodec" "avformat" "avutil")
  include_directories (${FFMPEG_DIR})
  add_definitions (-DFFMPEG_ENABLED)
endif (FFMPEG_FOUND)

add_executable (pitch_detection ${SOURCE_FILES})
target_link_libraries (pitch_detection ${LIBS})
