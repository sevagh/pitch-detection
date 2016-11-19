cmake_minimum_required(VERSION 3.1)
project(pitch_detection)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB SOURCE_FILES 
    "main.cpp"
    "include/*.h"
    "src/*.cpp"
)

add_executable(pitch_detection ${SOURCE_FILES})

target_link_libraries (pitch_detection fftw3 avcodec avformat avutil)

include_directories("./include")
