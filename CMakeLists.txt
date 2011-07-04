cmake_minimum_required(VERSION 2.8)
project(stepper-control CXX)

find_package( Boost 1.44 COMPONENTS filesystem program_options system)

#message(STATUS "Adding include directory ${CMAKE_INSTALL_PREFIX}/include")
#include_directories("${CMAKE_INSTALL_PREFIX}/include")
#link_directories("${CMAKE_INSTALL_PREFIX}/lib")

set(CMAKE_CXX_FLAGS "-Wall -std=c++0x -O3")
set(CMAKE_CXX_FLAGS_DEBUG "-Wextra")

set(CMAKE_EXE_LINKER_FLAGS " -lctb-0.15 -lpthread -lboost_system -lboost_regex")


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
