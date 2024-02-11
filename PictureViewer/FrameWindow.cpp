#include "FrameWindow.h"
#include "FrameWindowImpl.h"

FrameWindow::FrameWindow(std::string windowName) :
    pFrameWindowImpl(new FrameWindowImpl(*this, windowName))
{
}

void FrameWindow::Init(int textureID)
{
	pFrameWindowImpl->Init(textureID);
}

void FrameWindow::Render()
{
	pFrameWindowImpl->Render();
}

void FrameWindow::Deinit()
{
	pFrameWindowImpl->DeInit();
}

std::string FrameWindow::GetName()
{
	return pFrameWindowImpl->GetName();
}

std::string FrameWindow::GetConfig()
{
	return pFrameWindowImpl->GetConfig();
}

void FrameWindow::SetConfig(std::string& config)
{
	pFrameWindowImpl->SetConfig(config);
}

void FrameWindow::AddGraphicalObject(GraphicalObject* pGraphicalObject)
{
	pFrameWindowImpl->AddGraphicalObject(pGraphicalObject);
}

void FrameWindow::AddGraphicalObjects(std::vector<GraphicalObject *> &graphicalObjects)
{
    pFrameWindowImpl->AddGraphicalObjects(graphicalObjects);
}

void FrameWindow::ChangeGraphicalObjectsColor(const Color& color, int groupId)
{
    pFrameWindowImpl->ChangeGraphicalObjectsColor(color, groupId);
}

void FrameWindow::ChangeGraphicalObjectsActiveness(bool active, int groupId)
{
	pFrameWindowImpl->ChangeGraphicalObjectsActiveness(active, groupId);
}

void FrameWindow::ChangeGraphicalObjectsVisibility(bool visibility, int groupId)
{
	pFrameWindowImpl->ChangeGraphicalObjectsVisibility(visibility, groupId);
}

void FrameWindow::ChangeGraphicalObjectsNoteVisibility(bool visibility, int groupId)
{
	pFrameWindowImpl->ChangeGraphicalObjectsNoteVisibility(visibility, groupId);
}

void FrameWindow::DeleteGraphicalObject(GraphicalObject* pGraphicalObject)
{
	pFrameWindowImpl->DeleteGraphicalObject(pGraphicalObject);
}

void FrameWindow::DeleteGraphicalObject(int id)
{
	pFrameWindowImpl->DeleteGraphicalObject(id);
}

std::vector<GraphicalObject*> FrameWindow::GetGraphicalObjects()
{
	return pFrameWindowImpl->GetGraphicalObjects();
}

void FrameWindow::SetName(std::string name)
{
	pFrameWindowImpl->SetName(name);
}

bool FrameWindow::IsWindowMovementDisabled()
{
	return pFrameWindowImpl->IsWindowMovementDisabled();
}

void FrameWindow::EnableControlElementsAdd(bool status)
{
	pFrameWindowImpl->EnableControlElementsAdd(status);
}

void FrameWindow::EnableGeometriesChangeNotification(bool status)
{
	pFrameWindowImpl->EnableGeometriesChangeNotification(status);
}

bool FrameWindow::IsControlElementsAddEnabled()
{
	return pFrameWindowImpl->IsControlElementsAddEnabled();
}

void FrameWindow::ShowWindowIDInTitle(bool status)
{
	pFrameWindowImpl->ShowWindowIDInTitle(status);
}

void FrameWindow::GetPanZoom(float& panX, float& panY, float& zoom)
{
	pFrameWindowImpl->GetPanZoom(panX, panY, zoom);
}

void FrameWindow::SetPanZoom(float panX, float panY, float zoom)
{
	pFrameWindowImpl->SetPanZoom(panX, panY, zoom);
}

FrameWindow::~FrameWindow()
{
	delete pFrameWindowImpl;
}

float FrameWindow::GetRescaleRatio()
{
	return pFrameWindowImpl->GetRescaleRatio();
}

void FrameWindow::UpdateFrame(const Frame& frame)
{
	pFrameWindowImpl->UpdateFrame(frame);
}

void FrameWindow::DeleteGraphicalObjects(int groupId)
{
    pFrameWindowImpl->DeleteGraphicalObjects(groupId);
}

