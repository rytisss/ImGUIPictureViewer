cmake_minimum_required(VERSION 3.17.0)

project(PictureViewer)
set(CMAKE_BUILD_PARALLEL_LEVEL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions( -DPICTUREVIEWER_EXPORT )
if (APPLE)
add_definitions( -DCMAKE_APPLE_SILICON_PROCESSOR=arm64 )
endif (APPLE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(OpenGL REQUIRED)
# boost
set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.79.0 COMPONENTS system serialization REQUIRED)

include_directories(
	${Boost_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIRS}
	GraphicalObject//
	Geometries//
	..//Utilities//
	..//imgui//
	..//imgui//imgui//
	..//imgui//third_party/glfw/include
	..//imgui//third_party/gl3w/
	..//imgui//third_party/glm-0.9.9.6/
 )

add_library(${PROJECT_NAME} 
	FrameWindow.h
	FrameWindow.cpp
	FrameWindowImpl.h
	FrameWindowImpl.cpp
	PictureViewer.h
	Color.h
	GraphicalObject//GraphicalObject.h
	GraphicalObject//GraphicalObject.cpp
	GraphicalObject//RectangleGraphics.h
	GraphicalObject//RectangleGraphics.cpp
	GraphicalObject//PolygonGraphics.h
	GraphicalObject//PolygonGraphics.cpp
	GraphicalObject//CircleGraphics.h
	GraphicalObject//CircleGraphics.cpp
	GraphicalObject//RotatedRectangleGraphics.h
	GraphicalObject//RotatedRectangleGraphics.cpp
	Geometries/Processing2D.cpp
	Geometries/Processing2D.h)

target_link_libraries(${PROJECT_NAME} PRIVATE
	Utilities
	imgui
		${OPENGL_LIBRARIES}
	${Boost_LIBRARIES})

#add_dependencies(${PROJECT_NAME}
#	Utilities
#	imgui
#	)