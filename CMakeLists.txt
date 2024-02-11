cmake_minimum_required (VERSION 3.17.0)
project (InspectorCV_application)
set(CMAKE_CXX_STANDARD 20)

if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
    add_definitions( -DCMAKE_APPLE_SILICON_PROCESSOR=arm64 )
endif (APPLE)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++0x)
endif()

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(CMAKE_BUILD_PARALLEL_LEVEL)

add_subdirectory(App)
add_subdirectory(Backend)
add_subdirectory(Core)
add_subdirectory(PictureViewer)
add_subdirectory(Utilities)
add_subdirectory(imgui)
add_subdirectory(imgui//third_party)

