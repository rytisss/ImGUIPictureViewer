#include "CircleGraphics.h"
#include <cstdlib>
#include "imgui.h"
#include <iostream>
#include "nlohmann/json.hpp"

CircleGraphics::CircleGraphics()
	: m_mouseOnObject(false)
	, m_mouseOnResizeHandle(false)
	, m_xOnClick(0.f)
	, m_yOnClick(0.f)
	, m_radiusOnClick(0.f)
{
}

CircleGraphics::CircleGraphics(const CircleGraphics& obj)
	: m_mouseOnObject(false)
	, m_mouseOnResizeHandle(false)
	, m_xOnClick(0.f)
	, m_yOnClick(0.f)
	, m_radiusOnClick(0.f)
{
	m_id = obj.m_id;
}

void CircleGraphics::Init(int id)
{
	m_id = id;
}

void CircleGraphics::DeInit()
{
}

void CircleGraphics::OnLeftDownInternal(float mouseX, float mouseY, const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio)
{
	float diffX = mouseX - m_mouseXOnLeftClick;
	float diffY = mouseY - m_mouseYOnLeftClick;
	if (m_mouseOnObject)
	{
		circle.center.x = m_xOnClick + diffX;
        circle.center.y = m_yOnClick + diffY;
	}
	else
	{
		diffX += m_radiusOnClick;
        circle.radius = std::abs(std::sqrt(std::pow(diffX, 2) + std::pow(diffY, 2)));
	}
}

void CircleGraphics::OnLeftClickInternal()
{
	m_xOnClick = circle.center.x;
	m_yOnClick = circle.center.y;
	m_radiusOnClick = circle.radius;
}

void CircleGraphics::OnLeftUpInternal()
{
	ResetMouseStateOnObject();
}

void CircleGraphics::OnRightDownInternal(float mouseX, float mouseY, const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio)
{
}

void CircleGraphics::OnRightClickInternal()
{
}

void CircleGraphics::OnRightUpInternal()
{
}

bool CircleGraphics::IsMouseOnObjectInternal(float mouseX,
	float mouseY,
	const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
	const float& canvasLeft,
	const float& canvasTop,
	const float& pixelRatio, 
	const float& resizeRatio)
{
	ResetMouseStateOnObject();
	m_mouseOnResizeHandle = IsMouseInResizeHandle(mouseX,
                                                  mouseY,
                                                  circle.center.x + circle.radius,
                                                  circle.center.y,
                                                  m_handleSize,
                                                  coordToCanvasConverter,
                                                  canvasLeft,
                                                  canvasTop,
                                                  pixelRatio,
                                                  resizeRatio);
	if (m_mouseOnResizeHandle)
	{
		return true;
	}
	m_mouseOnObject = IsMouseInCircle(mouseX, mouseY);
	if (m_mouseOnObject)
	{
		return true;
	}
	else
	{
	}
	return false;
}

CircleGraphics::~CircleGraphics()
{
}

void CircleGraphics::CalculateCircleCoord(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
	const float& canvasLeft,
	const float& canvasTop,
	float& x_,
	float& y_,
	float& radius_,
	const float& pixelRatio,
	const float& resizeRatio)
{
	coordToCanvasConverter(circle.center.x, circle.center.y, x_, y_);
	radius_ = circle.radius * pixelRatio;
	x_ += canvasLeft;
	y_ += canvasTop;
}

void CircleGraphics::CalculateHandleRectangleCoord(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
	const float& canvasLeft,
	const float& canvasTop,
	const float& handleCenterX,
	const float& handleCenterY,
	float& left,
	float& right,
	float& top,
	float& bottom,
	const float& pixelRatio)
{
	float halfHandleSize = m_handleSize / pixelRatio / 2.f;
	float centerX = 0.f;
	float centerY = 0.f;
	coordToCanvasConverter(handleCenterX, handleCenterY, centerX, centerY);
	left = canvasLeft + centerX - halfHandleSize;
	right = canvasLeft + centerX + halfHandleSize;
	top = canvasTop + centerY - halfHandleSize;
	bottom = canvasTop + centerY + halfHandleSize;
}

void CircleGraphics::DrawCircleHandle(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
	const float& canvasLeft,
	const float& canvasTop,
	const float& pixelRatio,
	const Color& color)
{
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	float TLLeft = 0.f;
	float TLRight = 0.f;
	float TLTop = 0.f;
	float TLBottom = 0.f;
	CalculateHandleRectangleCoord(coordToCanvasConverter,
                                  canvasLeft,
                                  canvasTop,
                                  circle.center.x + circle.radius,
                                  circle.center.y,
                                  TLLeft,
                                  TLRight,
                                  TLTop,
                                  TLBottom,
                                  1.f);
	drawList->AddRectFilled(ImVec2(TLLeft, TLTop), ImVec2(TLRight, TLBottom), IM_COL32(color.R, color.G, color.B, color.A));
	if (m_mouseOnResizeHandle)
	{
		drawList->AddRect(ImVec2(TLLeft, TLTop), ImVec2(TLRight, TLBottom), IM_COL32(255,255,255,255));
	}
}

void CircleGraphics::DrawCircle(float x,
	float y,
	float radius,
	const float& pixelRatio,
	Color color,
    bool drawHighlight)
{
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	if (drawHighlight)
	{
		drawList->AddCircle(ImVec2(x, y), radius, IM_COL32(255,255,255,150), 0, 1.5f);
	}
	drawList->AddCircle(ImVec2(x, y), radius, IM_COL32(color.R, color.G, color.B, color.A));
}

void CircleGraphics::RenderID(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
                              const float& canvasLeft,
                              const float& canvasTop,
                              const float& pixelRatio,
                              const float& resizeRatio)
{
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	float canvasX = 0.f;
	float canvasY = 0.f;
	coordToCanvasConverter(circle.center.x, circle.center.y + circle.radius, canvasX, canvasY);
	ImVec2 textPos(canvasX + canvasLeft, canvasY + canvasTop);
	std::string text = "ID" + std::to_string(m_id);
	//calculate text width and height
	ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
	//set text to be in the middle
	textPos.x -= textSize.x / 2.f;
	textPos.y -= (textSize.y + 1.f);
	//render transparent background for text
	drawList->AddRectFilled(ImVec2(textPos.x - 1.f, textPos.y),
		ImVec2(textPos.x + textSize.x + 1.f, textPos.y + textSize.y),
		IM_COL32(100, 100, 100, 150),
		0);
	//render text
	drawList->AddText(textPos, IM_COL32_WHITE, text.c_str());
}

void CircleGraphics::Offset(float x, float y)
{
	circle.center.x += x;
    circle.center.y += y;
}

std::string CircleGraphics::GetConfig()
{
	//x, y, radius values needs to be added to the JSON along with ID and Title
	nlohmann::json circleConfig;
	circleConfig["x"] = circle.center.x;
	circleConfig["y"] = circle.center.y;
	circleConfig["radius"] = circle.radius;
	circleConfig["title"] = this->title;
	circleConfig["id"] = this->m_id;
	circleConfig["renderID"] = this->renderID;
	circleConfig["group"] = this->m_groupIndex;
	circleConfig["activate"] = this->m_active;
	circleConfig["visible"] = this->m_visible;
	circleConfig["type"] = "CircleGraphics";
	std::string circleConfig_str = circleConfig.dump();
	return circleConfig_str;
}

void CircleGraphics::SetConfig(std::string& config)
{
	//assign every value from config
	try
	{
		nlohmann::json circleConfig = nlohmann::json::parse(config);
		circleConfig.at("x").get_to(circle.center.x);
		circleConfig.at("y").get_to(circle.center.y);
		circleConfig.at("radius").get_to(circle.radius);
		circleConfig.at("title").get_to(this->title);
		circleConfig.at("id").get_to(this->m_id);
		circleConfig.at("renderID").get_to(this->renderID);
		circleConfig.at("group").get_to(this->m_groupIndex);
		circleConfig.at("activate").get_to(this->m_active);
		circleConfig.at("visible").get_to(this->m_visible);
	}
	catch (...)
	{
		std::cout << "Failed to load configuration in Circle object!" << std::endl;
	}
}

void CircleGraphics::Transform(float x, float y, float scaleX, float scaleY)
{
}

bool CircleGraphics::operator==(const GraphicalObject& obj)
{
	//check sanity
	const CircleGraphics* right = dynamic_cast<const CircleGraphics*>(&obj);
	bool result = false;
	if (right != nullptr)
	{
		result = right->circle.center.x == this->circle.center.x &&
			right->circle.center.y == this->circle.center.y &&
			right->circle.radius == this->circle.radius;
	}
	return result;
}

CircleGraphics& CircleGraphics::operator=(const CircleGraphics& obj)
{
	//check sanity
	if (&obj != this)
	{
		const CircleGraphics* right = dynamic_cast<const CircleGraphics*>(&obj);
		if (right != nullptr)
		{
			this->circle.center.x = right->circle.center.x;
			this->circle.center.y = right->circle.center.y;
			this->circle.radius = right->circle.radius;
		}
		else
		{
			#if defined(_WIN32)
				throw std::exception("Circular graphics are being assigned to different type!");
			#endif
		}
	}
	return *this;
}

void CircleGraphics::RenderWhenFocused(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio)
{
	//calculate position of rectangular
	float x_ = 0.f;
	float y_ = 0.f;
	float radius_ = 0.f;
	CalculateCircleCoord(coordToCanvasConverter, canvasLeft, canvasTop, x_, y_, radius_, pixelRatio, resizeRatio);
	Color color = GetGlowingColor();
	DrawCircle(x_, y_, radius_, pixelRatio, color, false);
	//DrawRectangleHandles(coordToCanvasConverter, canvasLeft, canvasTop, m_focusColor);
	if (renderID)
	{
		RenderID(coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio);
	}
}

void CircleGraphics::RenderWhenControlled(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio)
{
	//calculate position of circle
	float x_ = 0.f;
	float y_ = 0.f;
	float radius_ = 0.f;
	CalculateCircleCoord(coordToCanvasConverter, canvasLeft, canvasTop, x_, y_, radius_, pixelRatio, resizeRatio);
	Color color = GetGlowingColor();
	DrawCircle(x_, y_, radius_, pixelRatio, color, false);
	DrawCircleHandle(coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, color);
	if (renderID)
	{
		RenderID(coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio);
	}
}

void CircleGraphics::RenderWhenMouseHoveredOver(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio)
{
	//calculate position of circle
	float x_ = 0.f;
	float y_ = 0.f;
	float radius_ = 0.f;
	CalculateCircleCoord(coordToCanvasConverter, canvasLeft, canvasTop, x_, y_, radius_, pixelRatio, resizeRatio);
	DrawCircle(x_, y_, radius_, pixelRatio, m_color, false);
	DrawCircleHandle(coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, m_hoverOverColor);
	if (renderID)
	{
		RenderID(coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio);
	}
}

void CircleGraphics::RenderSimple(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio)
{
	//calculate position of circle
	float x_ = 0.f;
	float y_ = 0.f;
	float radius_ = 0.f;
	CalculateCircleCoord(coordToCanvasConverter, canvasLeft, canvasTop, x_, y_, radius_, pixelRatio, resizeRatio);
	DrawCircle(x_, y_, radius_, pixelRatio, m_color, false);
	if (renderID)
	{
		RenderID(coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio);
	}
}

bool CircleGraphics::IsMouseInResizeHandle(float mouseX,
	float mouseY,
	float handleCenterX,
	float handleCenterY,
	float handleSize,
	const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
	const float& canvasLeft,
	const float& canvasTop,
	const float& pixelRatio,
	const float& resizeRatio)
{
	float halfHandleSize = m_handleSize / pixelRatio / 2.f;
	float left = handleCenterX - halfHandleSize;
	float right = handleCenterX + halfHandleSize;
	float top = handleCenterY + halfHandleSize;
	float bottom = handleCenterY - halfHandleSize;
	//CalculateHandleRectangleCoord(coordToCanvasConverter, canvasLeft, canvasTop, handleCenterX, handleCenterY, left, right, top, bottom, pixelRatio);
	return (left <= mouseX && right >= mouseX && bottom <= mouseY && top >= mouseY);
}

bool CircleGraphics::IsMouseInCircle(float mouseX, float mouseY)
{
	return std::sqrt(std::pow(mouseX - circle.center.x, 2) +
    std::pow(mouseY - circle.center.y, 2)) <= circle.radius;
}

void CircleGraphics::ResetMouseStateOnObject()
{
	m_mouseOnResizeHandle = false;
	m_mouseOnObject = false;
}

void CircleGraphics::SetGeometryOnWholeImage(int imageWidth, int imageHeight)
{
	circle.center.x = (float)imageWidth / 2.f;
    circle.center.y = (float)imageHeight / 2.f;
    circle.radius = (float)std::min(imageWidth, imageHeight) / 2.f;
}
