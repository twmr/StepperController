add_definitions(-DSERIAL_DEBUG)

set(CMAKE_CXX_FLAGS "-Wall -std=c++0x -O3")
set(CMAKE_CXX_FLAGS_DEBUG "-Wextra")

add_executable(
	moves_gui
		iap_gui_app.cpp
		pm301.cpp
)
include_directories( 
	${PROJECT_SOURCE_DIR}/include
	${wxWidgets_INCLUDE_DIRS} 
)
TARGET_LINK_LIBRARIES(
	moves_gui
		${wxWidgets_LINK_FLAGS}
		${wxWidgets_LIBRARIES}
)
# ${wxWidgets_LIBRARIES} pangocairo-1.0 X11 cairo)
