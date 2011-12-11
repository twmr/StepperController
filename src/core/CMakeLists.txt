add_definitions(-DSERIAL_DEBUG)

set(CMAKE_CXX_FLAGS "-Wall -std=c++0x -O3")
set(CMAKE_CXX_FLAGS_DEBUG "-Wextra")

add_executable(
	moves_server
		rs232.cpp
		ctb.cpp
		helper.cpp
		IAPconfig.cpp
		IAPBoard.cpp
		IAP_server.cpp
)
target_link_libraries(moves_server ${CTB_LIBRARY} boost_system boost_regex boost_program_options pthread)

add_executable(
	moves_client_cli
		IAP_client_cli.cpp
)
target_link_libraries(moves_client_cli ${CTB_LIBRARY} boost_system
	boost_program_options boost_thread-mt pthread readline)

add_executable(
  moves_egun_cli
  rs232.cpp
  ctb.cpp
  egun_cli.cpp
)
target_link_libraries(moves_egun_cli ${CTB_LIBRARY} boost_system
	boost_program_options pthread readline)
