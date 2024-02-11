#ifndef CONTEXTWINDOW_H
#define CONTEXTWINDOW_H
// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <cstdio>

// About OpenGL function loaders: modern OpenGL doesn't have a standard header file and requires individual function pointers to be loaded manually.
// Helper libraries are often used for this purpose! Here we are supporting a few common ones: gl3w, glew, glad.
// You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.

#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

#include "FrameWindow.h"
#include <mutex>


class ContextWindow
{
public:
	ContextWindow();
	int Init();
	int Run();
	void Deinit();
	~ContextWindow();
 private:
	// OpenGL error callback
	static void glfw_error_callback(int error, const char* description);
	//The main window context
	GLFWwindow* m_window;
	//background color
	ImVec4 m_backgroundColor;
	//unique windows ID generator, just increment index
	unsigned int GetTextureID();
	//Custom style
	void ActivateStyle();
    // load font
    bool LoadFont(std::string path = "res//Roboto-Medium.ttf");
    // Get graphical windows configuration
    std::string GetConfig();
    // Set config to windows
    void SetConfig(std::string& config);
    // Configuration file path
    std::string m_configPath;
	//current ID
	unsigned int m_textureID;
    //TODO: do we really need all of them separately of just a vector with pointers?
	//Image visualization window
	FrameWindow* m_frameWidow;
};

#endif //CONTEXTWINDOW_H
