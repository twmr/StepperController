cmake_minimum_required(VERSION 2.8)
project(stepper-control CXX)

set( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules )

# include some macros
include(macros)

find_package( Boost 1.44 COMPONENTS filesystem program_options system)
include_directories ( ${Boost_INCLUDE_DIRS} )
link_directories ( ${Boost_LIBRARY_DIRS} )

#TODO: find solution for systems/platforms without a readline lib
find_package( Readline REQUIRED)

find_package( CTB REQUIRED)
include_directories( ${CTB_INCLUDE_DIRS} )
message( STATUS "CTB_INCLUDE_DIRS = ${CTB_INCLUDE_DIRS}" )
message( STATUS "CTB_LIBRARY = ${CTB_LIBRARY}" )

#message(STATUS "Adding include directory ${CMAKE_INSTALL_PREFIX}/include")
#include_directories("${CMAKE_INSTALL_PREFIX}/include")
#link_directories("${CMAKE_INSTALL_PREFIX}/lib")

find_package(wxWidgets 2.9.2 COMPONENTS core base net)
IF(wxWidgets_FOUND)

 if(wxWidgets_DEFINITIONS)
  string(REGEX REPLACE "-D" ""
    wxWidgets_DEFINITIONS_woD
    "${wxWidgets_DEFINITIONS}"
  )
  set_property(DIRECTORY APPEND
                PROPERTY COMPILE_DEFINITIONS ${wxWidgets_DEFINITIONS_woD})
endif(wxWidgets_DEFINITIONS)

if(wxWidgets_DEFINITIONS_DEBUG)
  string(REGEX REPLACE "-D" ""
    wxWidgets_DEFINITIONS_DEBUG_woD
    "${wxWidgets_DEFINITIONS_DEBUG}"
  )
  set_property(DIRECTORY APPEND
                PROPERTY COMPILE_DEFINITIONS_DEBUG ${wxWidgets_DEFINITIONS_DEBUG_woD})
endif(wxWidgets_DEFINITIONS_DEBUG)

cmake_link_flags(wxWidgets_LINK_FLAGS "${wxWidgets_LIBRARIES}")

   add_subdirectory(src/gui)
ENDIF(wxWidgets_FOUND)

include_directories(include)

add_subdirectory(src/core)
