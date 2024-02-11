cmake_minimum_required(VERSION 3.20.0)

project(PictureViewerDemo)
set(CMAKE_BUILD_PARALLEL_LEVEL)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (APPLE)
	add_definitions( -DCMAKE_APPLE_SILICON_PROCESSOR=arm64 )
endif(APPLE)

include_directories(
	../Backend/
 )

add_executable(${PROJECT_NAME} 
	main.cpp
	)

add_dependencies(${PROJECT_NAME}
	Backend)

target_link_libraries(${PROJECT_NAME} PRIVATE
	Backend)

if (WIN32)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different
		${OpenCV_DIR}//x64//vc17//bin//$<$<CONFIG:debug>:opencv_core460d.dll>$<$<CONFIG:release>:opencv_core460.dll>
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different
		${OpenCV_DIR}//x64//vc17//bin//$<$<CONFIG:debug>:opencv_imgproc460d.dll>$<$<CONFIG:release>:opencv_imgproc460.dll>
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different
		${OpenCV_DIR}//x64//vc17//bin//$<$<CONFIG:debug>:opencv_imgcodecs460d.dll>$<$<CONFIG:release>:opencv_imgcodecs460.dll>
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different
		${OpenCV_DIR}//x64//vc17//bin//$<$<CONFIG:debug>:opencv_videoio460d.dll>$<$<CONFIG:release>:opencv_videoio460.dll>
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different
		${spdlog_DIR}//..//..//..//bin//$<$<CONFIG:debug>:spdlogd.dll>$<$<CONFIG:release>:spdlog.dll>
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif (WIN32)