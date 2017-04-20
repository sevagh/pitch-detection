project (pitch_detection CXX)
cmake_minimum_required (VERSION 3.1)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi -pedantic -Werror -Wall -O3 -std=c++17")

set (SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set (INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set (LIB_LIST "${SRC_DIR}/yin.cpp;./src/mpm.cpp;./src/autocorrelation.cpp;./src/parabolic_interpolation.cpp;./src/sinewave.cpp;./src/types.cpp")

include_directories ("${SOURCE_DIR}")

# find third party modules
set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)
find_package (xcorr REQUIRED)
# end find third party modules

if (NOT XCORR_FOUND)
  MESSAGE(FATAL_ERROR "xcorr required")
endif ()

set (LIBS "-lxcorr")

add_library (pitch_detection SHARED ${SOURCE_FILES})
target_link_libraries (pitch_detection ${LIBS})
install (TARGETS pitch_detection DESTINATION /usr/local/lib)
install (FILES ${INCLUDE_DIR}/pitch_detection.h DESTINATION /usr/local/include)
