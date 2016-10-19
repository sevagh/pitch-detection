cmake_minimum_required(VERSION 3.1)
project(pitch_detection)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(pitch_detection ${SOURCE_FILES} lib/sinewave/sinegenerator.cpp lib/sinewave/sinegenerator.h
                                    lib/mpm/mpm.h lib/mpm/mpm.cpp
                                    lib/goertzel/goertzel.cpp lib/goertzel/goertzel.h
                                    lib/dft/dft.cpp lib/dft/dft.h
                                    lib/autocorrelation/autocorrelation.cpp
                                    lib/autocorrelation/autocorrelation.h
                                    lib/helper/helper.h lib/helper/helper.cpp)

target_link_libraries (pitch_detection fftw3)
