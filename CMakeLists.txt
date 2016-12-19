project(pitch_detection)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi -pedantic -Werror -Wall -O3 -std=c++17")
set(SOURCE_DIR "./src")
set(SOURCE_FILES "./main.cpp;./src/yin.cpp;./src/yin.h;./src/mpm.cpp;./src/mpm.h;./src/sine.cpp;./src/sine.h;./src/pitch.cpp;./src/pitch.h;./src/helper.cpp;./src/helper.h;./src/goertzel.cpp;./src/goertzel.h;./src/dft.cpp;./src/dft.h;./src/mpm_yin_common.cpp;./src/mpm_yin_common.h")
set(FFMPEG_FILES "./src/file.cpp;./src/file.h")
set(FFTW_FILES "./src/autocorrelation.cpp;./src/autocorrelation.h")
set(LIBS "gflags")

include_directories("${SOURCE_DIR}")

# find third party modules
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)
find_package(gflags REQUIRED)
find_package(Ffmpeg)
find_package(FFTW)
# end find third party modules

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
