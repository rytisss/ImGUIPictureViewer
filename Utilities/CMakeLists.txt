cmake_minimum_required(VERSION 3.17.0)

add_definitions( -DCMAKE_APPLE_SILICON_PROCESSOR=arm64 )

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
project(Utilities C CXX)
set(CMAKE_BUILD_PARALLEL_LEVEL)

# boost
set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package (Boost 1.79.0 COMPONENTS system filesystem REQUIRED)

include_directories(
	${Boost_INCLUDE_DIR}
)

add_library(${PROJECT_NAME}
	DataParseUtil.h
	DataParseUtil.cpp
	Timer.h
	Timer.cpp
	Frame.h
	Frame.cpp
	NiftyMath.h
	NiftyMath.cpp
	tinyfiledialogs/tinyfiledialogs.h
	tinyfiledialogs/tinyfiledialogs.c
	)

target_link_libraries(${PROJECT_NAME} PRIVATE
	${Boost_LIBRARIES})

