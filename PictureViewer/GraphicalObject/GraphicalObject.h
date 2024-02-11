#ifndef GRAPHICALOBJECT_H
#define GRAPHICALOBJECT_H

#include <functional>
#include <vector>
#include "nlohmann/json.hpp"
#include "../Color.h"

//Interface for graphical rendering objects - interactive geometries
class GraphicalObject
{
public:
	GraphicalObject();
	GraphicalObject(GraphicalObject& obj);
	virtual std::string GetConfig() = 0;
	virtual void SetConfig(std::string& config) = 0;
	virtual void Init(int id) = 0;
	virtual void RenderWhenFocused(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
		const float& canvasLeft,
		const float& canvasTop,
		const float& pixelRatio,
		const float& resizeRatio) = 0;
	virtual void RenderWhenControlled(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
		const float& canvasLeft,
		const float& canvasTop,
		const float& pixelRatio,
		const float& resizeRatio) = 0;
	virtual void RenderWhenMouseHoveredOver(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
		const float& canvasLeft,
		const float& canvasTop,
		const float& pixelRatio,
		const float& resizeRatio) = 0;
	virtual void RenderSimple(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
		const float& canvasLeft,
		const float& canvasTop,
		const float& pixelRatio,
		const float& resizeRatio) = 0;
	Color GetColor();
    void SetColor(const Color& color);
	void Render(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
		const float& canvasLeft,
		const float& canvasTop,
		const float& pixelRatio,
		const float& resizeRatio); // pass 
	virtual void DeInit() = 0;
	int GetID();
	void SetID(int id);
	int GetGroup();
	void SetGroup(int groupIndex);
	void SetNote(const std::map<std::string, std::string>& notes);
	std::map<std::string, std::string> GetNote();
	void ChangeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void ChangeFocusColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void ChangeControlColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void ChangeHoverOverColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void SetFocus(bool state);
	void SetControl(bool state);
	void Active(bool state);
	void Visible(bool state);
	void NotesVisible(bool state);
	void SetHandleSize(float handleSize);
	float GetHandleSize(float handleSize);
	void ShowID(bool status);
	bool IsMouseOnObject(float mouseX,
		float mouseY,
		const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
		const float& canvasLeft,
		const float& canvasTop,
		const float& pixelRatio,
		const float& resizeRatio);
	void OnLeftDown(float mouseX, float mouseY, const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio,
		const float& resizeRatio);
	void OnLeftClick();
	void OnLeftUp();
	void OnRightDown(float mouseX, float mouseY, const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio,
		const float& resizeRatio);
	void OnRightClick();
	void OnRightUp();
	virtual void Offset(float x, float y) = 0;
	void MouseCursorHoverUpdate(float mouseX, float mouseY,
		const float& resizeRatio);
	void MouseLeftClickUpdate(float mouseX, float mouseY,
		const float& resizeRatio);
	void MouseRightClickUpdate(float mouseX, float mouseY,
		const float& resizeRatio);
	void MouseHoveredOver(bool state);
	void MouseLeftState(bool state);
	void MouseRightState(bool state);
	bool IsMouseHoveredOver();
	bool IsMouseLeftClickedOn();
	bool IsMouseRightClickedOn();
	bool IsInitialized();
	bool IsActive();
	bool IsControlled();
	bool IsVisible();
	bool IsFocused();
	bool IsNoteVisible();
	long GetLastUpdateTime();
	void UpdateTime();
	Color GetGlowingColor();
	//Readjust geometry to cover (or at least try to) whole image
	virtual void SetGeometryOnWholeImage(int imageWidth, int imageHeight) = 0;
	virtual void Transform(float x, float y, float scaleX = 1.f, float scaleY = 1.f) = 0;
	virtual ~GraphicalObject() = 0;
	std::string title;
	bool renderID;
protected:
	int m_id;
	bool m_focused;
	int m_groupIndex; // assigned specific object to defined group (it can be specific sport, control level or any
    // other)
	bool m_controlled;
	bool m_active;
	bool m_visible;
	bool m_noteVisible;
	long m_lastUpdateTime;
	bool m_mouseHoveredOver;
	bool m_leftMouseClickedOn;
	bool m_rightMouseClickedOn;
	float m_mouseXHoveredOver;
	float m_mouseYHoveredOver;
	float m_mouseXOnLeftClick;
	float m_mouseYOnLeftClick;
	float m_mouseXOnRightClick;
	float m_mouseYOnRightClick;
	float m_handleSize;
	Color m_color;
	float m_thickness;
	Color m_focusColor;
	Color m_controlColor;
	Color m_hoverOverColor;
	// notes as value and key, both as std::string
	std::map<std::string, std::string> m_note;
	virtual void RenderID(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
		const float& canvasLeft,
		const float& canvasTop,
		const float& pixelRatio,
		const float& resizeRatio = 1.f) = 0;
	virtual void OnLeftDownInternal(float mouseX, float mouseY, const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio,
		const float& resizeRatio) = 0;
	virtual void OnLeftClickInternal() = 0;
	virtual void OnLeftUpInternal() = 0;
	virtual void OnRightDownInternal(float mouseX, float mouseY, const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio,
		const float& resizeRatio) = 0;
	virtual void OnRightClickInternal() = 0;
	virtual void OnRightUpInternal() = 0;
	virtual bool IsMouseOnObjectInternal(float mouseX,
		float mouseY,
		const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
		const float& canvasLeft,
		const float& canvasTop,
		const float& pixelRatio,
		const float& resizeRatio) = 0;
};

#endif //GRAPHICALOBJECT_H