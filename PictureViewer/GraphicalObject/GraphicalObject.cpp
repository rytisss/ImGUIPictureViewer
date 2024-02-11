#include "GraphicalObject.h"
#include <chrono>

#define COLOR_R 255
#define COLOR_G 255
#define COLOR_B 0
#define COLOR_A 150

#define CONTROLCOLOR_R 20
#define CONTROLCOLOR_G 255
#define CONTROLCOLOR_B 20
#define CONTROLCOLOR_A 200

#define FOCUSCOLOR_R 20
#define FOCUSCOLOR_G 200
#define FOCUSCOLOR_B 20
#define FOCUSCOLOR_A 220

#define HOVEROVERCOLOR_R 20
#define HOVEROVERCOLOR_G 200
#define HOVEROVERCOLOR_B 20
#define HOVEROVERCOLOR_A 150

#define HANDLESIZE 12.f

GraphicalObject::GraphicalObject()
	: m_id(-1)
	, m_lastUpdateTime(0)
	, m_focused(false)
	, m_groupIndex(-1)
	, m_controlled(false)
	, m_active(true)
	, m_visible(true)
	, m_noteVisible(true)
	, m_mouseHoveredOver(false)
	, m_leftMouseClickedOn(false)
	, m_rightMouseClickedOn(false)
	, m_mouseXHoveredOver(0.f)
	, m_mouseYHoveredOver(0.f)
	, m_mouseXOnLeftClick(0.f)
	, m_mouseYOnLeftClick(0.f)
	, m_mouseXOnRightClick(0.f)
	, m_mouseYOnRightClick(0.f)
	, m_handleSize(HANDLESIZE)
	, m_color(COLOR_R, COLOR_G, COLOR_B, COLOR_A)
	, m_controlColor(CONTROLCOLOR_R, CONTROLCOLOR_G, CONTROLCOLOR_B, CONTROLCOLOR_A)
	, m_focusColor(FOCUSCOLOR_R, FOCUSCOLOR_G, FOCUSCOLOR_B, FOCUSCOLOR_A)
	, m_hoverOverColor(HOVEROVERCOLOR_R, HOVEROVERCOLOR_G, HOVEROVERCOLOR_B, HOVEROVERCOLOR_A)
	, title("no_name")
	, renderID(false)
	, m_thickness(1.f)
{
}

GraphicalObject::GraphicalObject(GraphicalObject& obj)
	: m_id(-1)
	, m_lastUpdateTime(0)
	, m_focused(false)
	, m_controlled(false)
	, m_active(true)
	, m_groupIndex(-1)
	, m_visible(true)
	, m_noteVisible(true)
	, m_mouseHoveredOver(false)
	, m_leftMouseClickedOn(false)
	, m_rightMouseClickedOn(false)
	, m_mouseXHoveredOver(0.f)
	, m_mouseYHoveredOver(0.f)
	, m_mouseXOnLeftClick(0.f)
	, m_mouseYOnLeftClick(0.f)
	, m_mouseXOnRightClick(0.f)
	, m_mouseYOnRightClick(0.f)
	, m_handleSize(HANDLESIZE)
	, m_color(COLOR_R, COLOR_G, COLOR_B, COLOR_A)
	, m_controlColor(CONTROLCOLOR_R, CONTROLCOLOR_G, CONTROLCOLOR_B, CONTROLCOLOR_A)
	, m_focusColor(FOCUSCOLOR_R, FOCUSCOLOR_G, FOCUSCOLOR_B, FOCUSCOLOR_A)
	, m_hoverOverColor(HOVEROVERCOLOR_R, HOVEROVERCOLOR_G, HOVEROVERCOLOR_B, HOVEROVERCOLOR_A)
	, title("no_name")
	, renderID(false)
{
}

Color GraphicalObject::GetColor()
{
	if (IsControlled()) return m_controlColor;
	if (IsFocused()) return m_focusColor;
	if (IsMouseHoveredOver()) return m_hoverOverColor;
	return m_color;
}

void GraphicalObject::SetColor(const Color& color)
{
    m_color = color;
}

void GraphicalObject::Render(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio)
{
	if (!IsVisible()) return;
	if (IsControlled())
	{
		RenderWhenControlled(coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio);
	}
	else if (IsMouseHoveredOver())
	{
		RenderWhenMouseHoveredOver(coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio);
	}
	else if (IsFocused())
	{
		RenderWhenFocused(coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio);
	}
	else
	{
		RenderSimple(coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio);
	}
	// maybe render differently when not active?
}


int GraphicalObject::GetID()
{
	return m_id;
}

void GraphicalObject::SetID(int id)
{
	m_id = id;
}

int GraphicalObject::GetGroup()
{
	return m_groupIndex;
}

void GraphicalObject::SetGroup(int groupIndex)
{
	m_groupIndex = groupIndex;
}

void GraphicalObject::SetNote(const std::map<std::string, std::string>& notes)
{
	m_note = notes;
}

std::map<std::string, std::string> GraphicalObject::GetNote()
{
	return m_note;
}

void GraphicalObject::ChangeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_color.R = r;
	m_color.G = g;
	m_color.B = b;
	m_color.A = a;
}

void GraphicalObject::ChangeFocusColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_focusColor.R = r;
	m_focusColor.G = g;
	m_focusColor.B = b;
	m_focusColor.A = a;
}

void GraphicalObject::ChangeControlColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_controlColor.R = r;
	m_controlColor.G = g;
	m_controlColor.B = b;
	m_controlColor.A = a;
}

void GraphicalObject::ChangeHoverOverColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_hoverOverColor.R = r;
	m_hoverOverColor.G = g;
	m_hoverOverColor.B = b;
	m_hoverOverColor.A = a;
}

void GraphicalObject::SetFocus(bool state)
{
	m_focused = state;
}

void GraphicalObject::SetControl(bool state)
{
	m_controlled = state;
	//'true' state of control activates focus
	if (state) { SetFocus(true); }
}

void GraphicalObject::Active(bool state)
{
	m_active = state;
}

void GraphicalObject::Visible(bool state)
{
	m_visible = state;
}

void GraphicalObject::NotesVisible(bool state)
{
	m_noteVisible = state;
}

void GraphicalObject::SetHandleSize(float handleSize)
{
	float minHandleSize = 0.5f;
	float maxHandleSize = 50.f;
	if (handleSize < minHandleSize)
	{
		m_handleSize = handleSize;
	}
	else if (handleSize > maxHandleSize)
	{
		m_handleSize = handleSize;
	}
	else
	{
		m_handleSize = handleSize;
	}
}

float GraphicalObject::GetHandleSize(float handleSize)
{
	return m_handleSize;
}

void GraphicalObject::ShowID(bool status)
{
	renderID = status;
}

bool GraphicalObject::IsMouseOnObject(float mouseX, float mouseY, const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio)
{
    if (!IsVisible()) return false;
	return IsMouseOnObjectInternal(mouseX, mouseY, coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio);
}

void GraphicalObject::OnLeftDown(float mouseX, float mouseY, const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio)
{
	if (!IsVisible()) return; // should be able to focus object???
	OnLeftDownInternal(mouseX, mouseY, coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio);
}

void GraphicalObject::OnLeftClick()
{
	if (!IsActive() || !IsVisible()) return;
	OnLeftClickInternal();
}

void GraphicalObject::OnLeftUp()
{
	if (!IsActive() || !IsVisible()) return;
	OnLeftUpInternal();
}

void GraphicalObject::OnRightDown(float mouseX, float mouseY, const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio)
{
	if (!IsActive() || !IsVisible()) return;
	OnRightDownInternal(mouseX, mouseY, coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio);
}

void GraphicalObject::OnRightClick()
{
	if (!IsActive() || !IsVisible()) return;
	OnRightClickInternal();
}

void GraphicalObject::OnRightUp()
{
	if (!IsActive() || !IsVisible()) return;
	OnRightUpInternal();
}

void GraphicalObject::MouseCursorHoverUpdate(float mouseX, float mouseY, const float& resizeRatio)
{
	if (!IsActive() || !IsVisible()) return;
	m_mouseXHoveredOver = mouseX;
	m_mouseYHoveredOver = mouseY;
}

void GraphicalObject::MouseLeftClickUpdate(float mouseX, float mouseY, const float& resizeRatio)
{
	if (!IsActive() || !IsVisible()) return;
	m_mouseXOnLeftClick = mouseX;
	m_mouseYOnLeftClick = mouseY;
}

void GraphicalObject::MouseRightClickUpdate(float mouseX, float mouseY, const float& resizeRatio)
{
	if (!IsActive() || !IsVisible()) return;
	m_mouseXOnRightClick = mouseX;
	m_mouseYOnRightClick = mouseY;
}

void GraphicalObject::MouseHoveredOver(bool state)
{
	if (!IsActive() || !IsVisible()) return;
	m_mouseHoveredOver = state;
}

void GraphicalObject::MouseLeftState(bool state)
{
	if (!IsActive() || !IsVisible()) return;
	m_leftMouseClickedOn = state;
	SetControl(state);
	if (state)
	{
		OnLeftClick();
		UpdateTime();
	}
	else
	{
		OnLeftUp();
	}
}

void GraphicalObject::MouseRightState(bool state)
{
	if (!IsActive() || !IsVisible()) return;
	m_rightMouseClickedOn = state;
	if (state)
	{
		OnRightClick();
		UpdateTime();
	}
	else
	{
		OnRightUp();
	}
}

bool GraphicalObject::IsMouseHoveredOver()
{
	return m_mouseHoveredOver;
}

bool GraphicalObject::IsMouseLeftClickedOn()
{
	return m_leftMouseClickedOn;
}

bool GraphicalObject::IsMouseRightClickedOn()
{
	return m_rightMouseClickedOn;
}

bool GraphicalObject::IsInitialized()
{
	return m_id != -1;
}

bool GraphicalObject::IsActive()
{
	return m_active;
}

bool GraphicalObject::IsControlled()
{
	return m_controlled;
}

bool GraphicalObject::IsVisible()
{
	return m_visible;
}

bool GraphicalObject::IsFocused()
{
	return m_focused;
}

bool GraphicalObject::IsNoteVisible()
{
	return m_noteVisible;
}

long GraphicalObject::GetLastUpdateTime()
{
	return m_lastUpdateTime;
}

void GraphicalObject::UpdateTime()
{
	m_lastUpdateTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch()).count();
}

Color GraphicalObject::GetGlowingColor()
{
	int timestamp = (int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() % 1000; //miliseconds only matter
	std::uint8_t firstGlowValue = (std::uint8_t)(std::sin((float)timestamp / 100.f) * 100.f + 100.f); //make value between 150 and 250
	std::uint8_t thirdGlowValue = (std::uint8_t)(std::cos((float)timestamp / 100.f) * 100.f + 150.f); //make value between 150 and 250
	return Color((int)firstGlowValue, 150, (int)thirdGlowValue, 255);
}

GraphicalObject::~GraphicalObject()
{
}
