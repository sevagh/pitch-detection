# FindFFTS.cmake

# Try to find FFTS
# Once done, this will define
# FFTS_FOUND - System has FFTS
# FFTS_INCLUDE_DIRS - The FFTS include directories
# FFTS_LIBRARIES - The libraries needed to use FFTS

find_path(FFTS_INCLUDE_DIR NAMES ffts/ffts.h)
find_library(FFTS_LIBRARY NAMES ffts)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FFTS DEFAULT_MSG FFTS_LIBRARY FFTS_INCLUDE_DIR)

if(FFTS_FOUND)
  set(FFTS_INCLUDE_DIRS ${FFTS_INCLUDE_DIR})
  set(FFTS_LIBRARIES ${FFTS_LIBRARY})
endif()
