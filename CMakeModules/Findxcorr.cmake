# - Find XCORR
# Find the native XCORR includes and library
#
#  XCORR_INCLUDES    - where to find xcorr.h
#  XCORR_LIBRARIES   - List of libraries when using XCORR.
#  XCORR_FOUND       - True if XCORR found.

if (XCORR_INCLUDES)
  # Already in cache, be silent
  set (XCORR_FIND_QUIETLY TRUE)
endif (XCORR_INCLUDES)

find_path (XCORR_INCLUDES xcorr.h _xcorr.h)
find_library (XCORR_LIBRARIES NAMES xcorr)

# handle the QUIETLY and REQUIRED arguments and set XCORR_FOUND to TRUE if
# all listed variables are TRUE
include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (XCORR DEFAULT_MSG XCORR_LIBRARIES XCORR_INCLUDES)

mark_as_advanced (XCORR_LIBRARIES XCORR_INCLUDES)
