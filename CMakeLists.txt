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

add_definitions(-DSERIAL_DEBUG)

set(CMAKE_CXX_FLAGS "-Wall -std=c++0x -O3")
set(CMAKE_CXX_FLAGS_DEBUG "-Wextra")

include_directories(include)

add_executable(
	moves_server
		rs232.cpp
		ctb.cpp
		helper.cpp
		IAPconfig.cpp
		IAPBoard.cpp
		IAP_server.cpp
)
target_link_libraries(moves_server ${CTB_LIBRARY} boost_system boost_regex boost_program_options)

add_executable(
	moves_client_cli
		IAP_client_cli.cpp
)
target_link_libraries(moves_client_cli ${CTB_LIBRARY} boost_system boost_program_options)
