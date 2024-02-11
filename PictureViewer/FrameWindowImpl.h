//This code is create by Rytis Augustauskas and it is distributed in https://github.com/rytisss/OpenGL3_ImGui_texture

#ifndef FRAMEWINDOWIMPL_H
#define FRAMEWINDOWIMPL_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GL/gl3w.h>
#include <atomic>
#include "Frame.h"
#include "GraphicalObject/GraphicalObject.h"
#include "FrameWindow.h"
#include <map>
#include <atomic>


class FrameWindow::FrameWindowImpl
{
public:
	FrameWindowImpl(FrameWindow& parent, std::string name);
	void Init(int id);
	void Render();
	void DeInit();
	void AddGraphicalObject(GraphicalObject* pGraphicalObject);
    void AddGraphicalObjects(std::vector<GraphicalObject*>& graphicalObjects);
	void ChangeGraphicalObjectsActiveness(bool active, int groupId = -1);
	void ChangeGraphicalObjectsVisibility(bool visibility, int groupId = -1);
    void ChangeGraphicalObjectsColor(const Color& color, int groupId = -1);
	void ChangeGraphicalObjectsNoteVisibility(bool visibility, int groupId = -1);
	void DeleteGraphicalObject(GraphicalObject* pGraphicalObject);
	void DeleteGraphicalObject(int id);
    void DeleteGraphicalObjects(int groupId=-1);
	std::vector<GraphicalObject*> GetGraphicalObjects();
	void SetName(std::string title);
	void SetAutoResize(bool state);
	int GetWindowWidth();
	int GetWindowHeight();
	bool IsWindowMovementDisabled();
	void EnableControlElementsAdd(bool status);
	void EnableGeometriesChangeNotification(bool status);
	bool IsControlElementsAddEnabled();
	void ShowWindowIDInTitle(bool status);
	void GetPanZoom(float& panX, float& panY, float& zoom);
	void SetPanZoom(float panX = 0.f, float panY = 0.f, float zoom = 1.f);
	std::string GetName();
	std::string GetConfig();
	void SetConfig(std::string& config);
	~FrameWindowImpl();
	float GetRescaleRatio();
	//deep copy
	void UpdateFrame(const Frame& frame);
private:
	//Parent object reference
	FrameWindow& m_parent;
	//first time rendering flag
	bool m_firstTime;
	//window title
	std::string m_title;
	//frame update lock
	std::mutex m_lock;
	//initialize status flag
	bool m_initialized;
	//last updated frame time stamp
	int64_t m_lastFrameCaptureTime;
	//frame buffer [ID]
	GLuint m_frameBuffer;
	//frame itself [original]
	Frame m_frame;
    //frame which size is reduced in case that image is bigger than max size handle by GPU
    //in case when original image fits to the screen, this is equal to the m_frame
    Frame m_fitFrame;
    //make a copy of the buffer reducing by integer, of just copy every nth element in
    //every column and row
    void DownscaleFrame(const Frame& src, Frame& dst, int nth);

	//width before resize
	int m_latestImageWidth;
	//height before resize
	int m_latestImageHeight;

	//width resized
	int m_latestResizedImageWidth;
	//width resized
	int m_latestResizedImageHeight;

	float m_latestAspectRatio;
	//flag to inspect if geometries in the window changed from previous rendering iteration
	bool m_checkGeometriesChange;
	//adjust window size on creation to prevent crash of too big clipped texture
	//returns true if texture/window size was adjusted
	void AdjustWindowSize();
	//For rendering
	//void Image2RGBA(const cv::Mat& input, cv::Mat& output);
	//update texture, put image to texture
	void UpdateTexture();
	//
	void HandlePan(float dx, float dy);
	//
	void SetPan(float panX, float panY);
	//
	void HandleZoom(float factor);
	//render pop-up if command is given
	void HandlePopUp();
	//Returns changed value
	float RenderInputParameterLine(float currentValue, std::string name, bool round = false);
	//Render graphical object note
	void RenderObjectNote(const std::map<std::string, std::string>& note);
	//
	void SetCanvasPosition();
	//
	void DrawContextWindow();
	//
	void RenderGraphicalObjectsParameters();
	//
	void CalculateQuad();
	//
	void HandleMouseManipulations();
	//
	void DrawGraphicalElements();
	//
	void CalculateMouseCoordinateOnTexture();
	//
	void UpdateConverter();
	//
	void ChangeCanvasColor(ImVec4 color);
	//
	void Canvas2TextureCoord(float canvasX, float canvasY, float& textureX, float& textureY);
	//
	void Texture2CanvasCoord(float textureX, float textureY, float& canvasX, float& canvasY);
	//actual one pixel in image size in canvas
	float m_latestPixelRatio;
	//Draw frame on canvas if focused
	void DrawFrameOnFocus();
	//Sort graphical element by newest clicked 
	void SortGraphicalObjectByUpdateTime();
	//Generate unique graphical object id
	unsigned int GetNextGraphicalObjectID();
	//Change graphical objects ids. After deleting a gap might appear in the number sequence. Change the id of graphical objects to be in the sequence
	void AdjustGraphicalObjectsIDs(int deletedID);
	//Upload start image
	void UploadInitialImage();
	//indication if canvas is focused
	bool m_canvasFocused;
	//Unique ID required for every window
	int m_windowID;
	//time to prepare all the object and add to drawing list
	float m_windowSetDuration;
	//show window id in title
	bool m_showWindowID;
	//window scaling
	float m_panX;
	float m_panY;
	float m_zoom;

	float m_zoom_factor;
	float m_pan_factor;

	//resize ratio
	float m_resizeRatio;
	//applied
	float m_resizeRatioApplied;

	//used to offset canvas when context column is shown
	float m_canvasXOffset;

	float m_mouseDownX;
	float m_mouseDownY;
	float m_panOnMouseDownX;
	float m_panOnMouseDownY;
	bool m_wasMouseClickedInsideCanvas;

	//when left mouse button down and 'ctrl' on keyboard is down
	float m_mouseDownCtrlX;
	float m_mouseDownCtrlY;
	//////////////////////////////////
	//canvas and texture parameters
	//////////////////////////////////
	ImVec4 m_canvasColor;
	ImVec4 m_infoTextColor;

	ImVec2 m_canvasTopLeft;
	ImVec2 m_canvasTopRight;
	ImVec2 m_canvasBottomRight;
	ImVec2 m_canvasBottomLeft;
	float m_canvasWidth{};
	float m_canvasHeight{};

	ImVec2 m_uv_0; //top left
	ImVec2 m_uv_1; //top right
	ImVec2 m_uv_2; //bottom right
	ImVec2 m_uv_3; //bottom left
	//Image width
	float m_imageWidth{};
	//Image height
	float m_imageHeight{};

	float m_textureWidthOffset{};
	float m_textureHeightOffset{};

	float m_widthRatio{}; //image width / canvas width * zoom
	float m_heightRatio{}; //image height / canvas height * zoom

	ImVec2 m_windowPos;
	ImVec2 m_windowSize;

	//latest mouse pos
	float m_mouseXInCanvas;
	float m_mouseYInCanvas;
	float m_mouseXInTexture;
	float m_mouseYInTexture;

	void ResetCanvasParameters();
	//Graphical elements vector (current)
	std::vector<GraphicalObject*> m_graphicalObjects;

	//Previous iteration graphical elements
	//It has deep copy of elements
	//Required for differences between object inspection
	std::vector<GraphicalObject*> m_oldGraphicalElements;
	//Copy current graphical objects to old for differences inspection
	void CopyCurrentGeometriesToOld();
	//Get changed graphical objects indices;
	std::vector<int> GetChangedGraphicalObjectIndices();

	//geometries in the window indices counter
	int m_geometriesIDCount;
    //map of graphical elements IDs and graphical element pointers [not occupied indices can be found more
    // easily/faster through this], ID will be registered though
    std::map<unsigned int, GraphicalObject*> m_graphicalObjectsMap;
    //Register graphical object ID (add to m_graphicalObjectsMap) and ID is returned
    unsigned int RegisterGraphicalObject(GraphicalObject* graphicalObject);
    //Unregister graphical object ID (remove from m_graphicalObjectsMap)
    void UnregisterGraphicalObject(GraphicalObject* graphicalObject);
	//Returns unique (incremented id)
	// int GetUniqueId();

	std::atomic_bool m_internalElementAddEnabled;
	//latest coordinates converter
	std::function<void(float, float, float&, float&)> m_canvas2TextureConverter;

    // maximum texture size by machine
    GLint m_maxTextureSize;
};

#endif //FRAMEWINDOWIMPL_H