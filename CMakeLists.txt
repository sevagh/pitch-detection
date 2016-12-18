project(pitch_detection)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi -pedantic -Werror -Wall -O3 -std=c++17")
set(SRC_DIR "./src")
set(FFMPEG_DIR "./src/ffmpeg")
set(FFTW_DIR "./src/fftw")
set(LIBS "gflags")

include_directories("${SRC_DIR}")

# find third party modules
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)
find_package(gflags REQUIRED)
find_package(Ffmpeg)
find_package(FFTW)
# end find third party modules

file(GLOB SOURCE_FILES 
    "main.cpp"
    "${SRC_DIR}/*.cpp"
    "${SRC_DIR}/*.h"
)

if (FFMPEG_FOUND)
  file(GLOB FFMPEG_ADDITIONAL
     "${FFMPEG_DIR}/*.h"
     "${FFMPEG_DIR}/*.cpp"
   )
  set(SOURCE_FILES "${SOURCE_FILES};${FFMPEG_ADDITIONAL}")
  LIST(APPEND LIBS "avcodec" "avformat" "avutil")
  include_directories(${FFMPEG_DIR})
  add_definitions(-DFFMPEG_ENABLED)
endif (FFMPEG_FOUND)

if (FFTW_FOUND)
  file(GLOB FFTW_ADDITIONAL
     "${FFTW_DIR}/*.h"
     "${FFTW_DIR}/*.cpp"
   )
  set(SOURCE_FILES "${SOURCE_FILES};${FFTW_ADDITIONAL}")
  LIST(APPEND LIBS "fftw3")
  include_directories(${FFTW_DIR})
  add_definitions(-DFFTW_ENABLED)
endif (FFTW_FOUND)

add_executable(pitch_detection ${SOURCE_FILES})
target_link_libraries(pitch_detection ${LIBS})
