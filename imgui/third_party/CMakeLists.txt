
# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
add_definitions( -DCMAKE_APPLE_SILICON_PROCESSOR=arm64 )
add_subdirectory(glfw)