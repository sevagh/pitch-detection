cmake_minimum_required(VERSION 3.10)
project(pitch_detection VERSION 1.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# use this for dependency graph generation
#set_property(GLOBAL PROPERTY GRAPHVIZ_EXPORT_TARGETS TRUE)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -ansi -pedantic -fext-numeric-literals -fopenmp")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto")

include_directories(include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/test)

find_package(mlpack REQUIRED)
find_package(FFTS REQUIRED)

# Assuming FFTS does not have a built-in find module

# Add the library target
file(GLOB_RECURSE LIB_SOURCES "src/*.cpp")
add_library(pitch_detection SHARED ${LIB_SOURCES})
target_link_libraries(pitch_detection PUBLIC
    ${MLPACK_LIBRARIES}
    ${FFTS_LIBRARIES})

find_package(gflags QUIET)
find_package(libnyquist QUIET)
find_package(PkgConfig QUIET)
pkg_search_module(OPUS QUIET opus)
pkg_search_module(WAVPACK QUIET wavpack)

if(gflags_FOUND AND OPUS_FOUND AND WAVPACK_FOUND AND libnyquist_FOUND)
    file(GLOB WAV_ANALYZER_SOURCES "wav_analyzer/*.cpp")
    add_executable(wav_analyzer ${WAV_ANALYZER_SOURCES})
    target_link_libraries(wav_analyzer PRIVATE
        pitch_detection
        ${GFLAGS_LIBRARIES}
        ${OPUS_LIBRARIES}
        ${WAVPACK_LIBRARIES}
        libnyquist)

    target_include_directories(wav_analyzer PRIVATE
        ${GFLAGS_INCLUDE_DIRS}
        ${OPUS_INCLUDE_DIRS}
        ${WAVPACK_INCLUDE_DIRS}
        ${LIBNYQUIST_INCLUDE_DIRS})

    link_directories(
        ${GFLAGS_LIBRARY_DIRS}
        ${OPUS_LIBRARY_DIRS}
        ${WAVPACK_LIBRARY_DIRS}
        ${LIBYNQUIST_LIBRARY_DIRS})
endif()

include(CTest)
find_package(GTest QUIET)
find_package(opus QUIET)
find_package(lib QUIET)
if(GTEST_FOUND)
    enable_testing()
    file(GLOB TEST_SOURCES "test/test*.cpp" "test/util.cpp")
    add_executable(pitch_tests ${TEST_SOURCES})
    target_link_libraries(pitch_tests PRIVATE
        pitch_detection
        GTest::GTest
        GTest::Main)
endif()

find_package(benchmark QUIET)
if(benchmark_FOUND)
    file(GLOB BENCH_SOURCES "test/bench.cpp" "test/util.cpp")
    add_executable(pitch_bench ${BENCH_SOURCES})
    target_link_libraries(pitch_bench PRIVATE
        pitch_detection
        benchmark::benchmark)
endif()

install(TARGETS pitch_detection LIBRARY DESTINATION lib)
install(FILES include/pitch_detection.h DESTINATION include)
