project(pitch_detection)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(FFMPEG_DIR "./ffmpeg-src")
set(FFTW_DIR "./fftw-src")
set(LIBS "")

include_directories("./include")

# find third party modules
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)
find_package(Ffmpeg)
find_package(FFTW)
# end find third party modules

file(GLOB SOURCE_FILES 
    "main.cpp"
    "include/*.h"
    "src/*.cpp"
)

if (FFMPEG_FOUND)
  include_directories(${FFMPEG_DIR})
  file(GLOB FFMPEG_ADDITIONAL
     "${FFMPEG_DIR}/*.h"
     "${FFMPEG_DIR}/*.cpp"
   )
  set(SOURCE_FILES "${SOURCE_FILES};${FFMPEG_ADDITIONAL}")
  LIST(APPEND LIBS "avcodec")
  LIST(APPEND LIBS "avformat")
  LIST(APPEND LIBS "avutil")
  include_directories(${FFMPEG_DIR})
  add_definitions(-DFFMPEG_ENABLED)
endif (FFMPEG_FOUND)

if (FFTW_FOUND)
  include_directories(${FFTW_DIR})
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
