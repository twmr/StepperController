cmake_minimum_required(VERSION 2.8)
project(stepper-control CXX)

find_package( Boost 1.44 COMPONENTS filesystem program_options system)

#message(STATUS "Adding include directory ${CMAKE_INSTALL_PREFIX}/include")
#include_directories("${CMAKE_INSTALL_PREFIX}/include")
#link_directories("${CMAKE_INSTALL_PREFIX}/lib")

set(EXTRA_FLAGS "-DSERIAL_DEBUG")

set(CMAKE_CXX_FLAGS "-Wall -std=c++0x -O3 ${EXTRA_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-Wextra")

#TODO replace this by something platform independent
set(CMAKE_EXE_LINKER_FLAGS " -lctb-0.15 -lpthread -lboost_system -lboost_program_options -lboost_regex")


add_executable(
	IAP_server
		rs232.cpp
		ctb.cpp
		helper.cpp
		IAPconfig.cpp
		IAPBoard.cpp
		IAP_server.cpp
)
add_executable(
	IAP_client_cli
		IAP_client_cli.cpp
)
include_directories(include)
include_directories ( ${Boost_INCLUDE_DIRS} )
link_directories ( ${Boost_LIBRARY_DIRS} )
