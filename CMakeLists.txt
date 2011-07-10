cmake_minimum_required(VERSION 2.8)
project(stepper-control CXX)

set( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules )



find_package( Boost 1.44 COMPONENTS filesystem program_options system)
include_directories ( ${Boost_INCLUDE_DIRS} )
link_directories ( ${Boost_LIBRARY_DIRS} )

find_package( CTB REQUIRED)
include_directories( ${CTB_INCLUDE_DIRS} )
message( STATUS "CTB_INCLUDE_DIRS = ${CTB_INCLUDE_DIRS}" )
message( STATUS "CTB_LIBRARY = ${CTB_LIBRARY}" )


#message(STATUS "Adding include directory ${CMAKE_INSTALL_PREFIX}/include")
#include_directories("${CMAKE_INSTALL_PREFIX}/include")
#link_directories("${CMAKE_INSTALL_PREFIX}/lib")


include_directories(include)

add_subdirectory(src/core)
