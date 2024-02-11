#ifndef FRAMEWINDOW_H
#define FRAMEWINDOW_H

#include "PictureViewer.h"
#include "GraphicalObject/GraphicalObject.h"
#include "RectangleGraphics.h"
#include "CircleGraphics.h"
#include "RotatedRectangleGraphics.h"
#include "PolygonGraphics.h"
#include "Color.h"
#include "Frame.h"

#include "boost/signals2.hpp"

//oldImageWidth, oldImageHeight, newImageWidth, newImageHeight 
typedef boost::signals2::signal<void(int, int, int, int)> OnImageSizeChange;
//geometry object IDs, -1 number of object changes
typedef boost::signals2::signal<void(std::vector<int>)> OnGeometryObjectChange;

class FrameWindow
{
public:
	explicit FrameWindow(std::string windowName);
	void Init(int textureID);
	void Render();
	void Deinit();
	void AddGraphicalObject(GraphicalObject* pGraphicalObject);
    void AddGraphicalObjects(std::vector<GraphicalObject*>& graphicalObjects);
    void ChangeGraphicalObjectsColor(const Color& color, int groupId=-1);
	void ChangeGraphicalObjectsActiveness(bool active, int groupId=-1);
	void ChangeGraphicalObjectsVisibility(bool visibility, int groupId=-1);
	void ChangeGraphicalObjectsNoteVisibility(bool visibility, int groupId=-1);
	void DeleteGraphicalObject(GraphicalObject* pGraphicalObject);
    void DeleteGraphicalObjects(int groupId=-1);
	void DeleteGraphicalObject(int id);
	std::vector<GraphicalObject*> GetGraphicalObjects();
    void SetName(std::string name);
	//Check if windows movement disables
	bool IsWindowMovementDisabled();
	void EnableControlElementsAdd(bool status);
	void EnableGeometriesChangeNotification(bool status);
	bool IsControlElementsAddEnabled();
	void ShowWindowIDInTitle(bool status);
	//cv::Size GetCurrentTextureSize();
	~FrameWindow();
	float GetRescaleRatio();
	void UpdateFrame(const Frame& frame);
	//Event generated when updated image is different size from previous
	OnImageSizeChange ImageSizeChange;
	//Event generate when geometry object internal parameter changes
	OnGeometryObjectChange OnGeometryObjectChange;
	void GetPanZoom(float& panX, float& panY, float& zoom);
	void SetPanZoom(float panX = 0.f, float panY = 0.f, float zoom = 1.f);
	
	std::string GetName();
    std::string GetConfig();
    void SetConfig(std::string& config);

private:
	class FrameWindowImpl;
	FrameWindowImpl* pFrameWindowImpl;
};

#endif //FRAMEWINDOW_H