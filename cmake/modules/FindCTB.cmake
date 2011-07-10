# Find CTB header and library.

# This module defines the following uncached variables:
#  CTB_FOUND, if false, do not try to use CTB.
#  CTB_INCLUDE_DIRS, where to find ctb.h.
#  CTB_LIBRARIES, the libraries to link against to use CTB
#  CTB_LIBRARY_DIRS, the directory where the CTB library is found.

FIND_PATH(
  CTB_INCLUDE_DIR
  ctb.h 
  /usr/local/include/ctb-0.16
)

IF( CTB_INCLUDE_DIR )
  find_library( CTB_LIBRARY
    NAMES ctb-0.16
    PATHS /usr/local/lib /usr/lib
		NO_CMAKE_ENVIRONMENT_PATH
	)
  if(CTB_LIBRARY)
    # Set uncached variables as per standard.
    set(CTB_FOUND ON)
    set(CTB_INCLUDE_DIRS ${CTB_INCLUDE_DIR})
    set(CTB_LIBRARIES ${CTB_LIBRARY})
  endif(CTB_LIBRARY)
  
endif(CTB_INCLUDE_DIR)
	    
if(CTB_FOUND)
  if(NOT CTB_FIND_QUIETLY)
    message(STATUS "FindCTB: Found both CTB headers and library")
  endif(NOT CTB_FIND_QUIETLY)
else(CTB_FOUND)
  if(CTB_FIND_REQUIRED)
    message(FATAL_ERROR "FindCTB: Could not find CTB headers or library")
  endif(CTB_FIND_REQUIRED)
endif(CTB_FOUND)

MARK_AS_ADVANCED(
  CTB_INCLUDE_DIR
  CTB_LIBRARY
)
