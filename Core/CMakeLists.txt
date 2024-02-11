cmake_minimum_required(VERSION 3.17.0)

project(Core)
set(CMAKE_BUILD_PARALLEL_LEVEL)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions( -DCORE_EXPORT )
if (APPLE)
	add_definitions( -DCMAKE_APPLE_SILICON_PROCESSOR=arm64 )
endif(APPLE)
# boost
set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.79.0 COMPONENTS system serialization REQUIRED)

include_directories(
	${OpenCV_INCLUDE_DIRS}
	${Boost_INCLUDE_DIR}
 )

add_library(Core SHARED 
	Dummy.h
	Dummy.cpp
	LimitedValue.h
	Core.h
	Color.h
	StateValue.h)

target_link_libraries(Core PRIVATE
	${OpenCV_LIBS}
	${Boost_LIBRARIES})


