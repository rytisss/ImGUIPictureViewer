#include "RectangleGraphics.h"
#include <cstdlib>
#include "imgui.h"
#include <iostream>

RectangularGraphics::RectangularGraphics()
	: x(0.f)
	, y(0.f)
	, width(0.f)
	, height(0.f)
	, m_mouseOnTopLeftHandle(false)
	, m_mouseOnTopRightHandle(false)
	, m_mouseOnBottomLeftHandle(false)
	, m_mouseOnBottomRightHandle(false)
	, m_mouseOnObject(false)
	, m_xOnClick(0.f)
	, m_yOnClick(0.f)
	, m_widthOnClick(0.f)
	, m_heightOnClick(0.f)
{
}

RectangularGraphics::RectangularGraphics(float x, float y, float width, float height, Color color, float thickness)
	: x(x)
	, y(y)
	, width(width)
	, height(height)
	, m_mouseOnTopLeftHandle(false)
	, m_mouseOnTopRightHandle(false)
	, m_mouseOnBottomLeftHandle(false)
	, m_mouseOnBottomRightHandle(false)
	, m_mouseOnObject(false)
	, m_xOnClick(0.f)
	, m_yOnClick(0.f)
	, m_widthOnClick(0.f)
	, m_heightOnClick(0.f)
{
	m_color = color;
	m_thickness = thickness;
}

RectangularGraphics::RectangularGraphics(const RectangularGraphics& obj)
	: x(obj.x)
	, y(obj.y)
	, width(obj.width)
	, height(obj.height)
	, m_mouseOnTopLeftHandle(false)
	, m_mouseOnTopRightHandle(false)
	, m_mouseOnBottomLeftHandle(false)
	, m_mouseOnBottomRightHandle(false)
	, m_mouseOnObject(false)
	, m_xOnClick(0.f)
	, m_yOnClick(0.f)
	, m_widthOnClick(0.f)
	, m_heightOnClick(0.f)
{
	m_id = obj.m_id;
}

void RectangularGraphics::Init(int id)
{
	m_id = id;
}

void RectangularGraphics::DeInit()
{
}

void RectangularGraphics::OnLeftDownInternal(float mouseX,
	float mouseY,
	const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
	const float& canvasLeft,
	const float& canvasTop,
	const float& pixelRatio,
	const float& resizeRatio)
{
	float diffX = mouseX - m_mouseXOnLeftClick;
	float diffY = mouseY - m_mouseYOnLeftClick;
	if (m_mouseOnObject)
	{
		x = m_xOnClick + diffX;
		y = m_yOnClick + diffY;
	}
	else
	{
		float left = m_xOnClick;
		float right = m_xOnClick + m_widthOnClick;
		float top = m_yOnClick;
		float bottom = m_yOnClick + m_heightOnClick;
		if (m_mouseOnTopLeftHandle)
		{
			left += diffX;
			top += diffY;
		}
		else if (m_mouseOnTopRightHandle)
		{
			right += diffX;
			top += diffY;
		}
		else if (m_mouseOnBottomLeftHandle)
		{
			left += diffX;
			bottom += diffY;
		}
		else if (m_mouseOnBottomRightHandle)
		{
			right += diffX;
			bottom += diffY;
		}
		x = left < right ? left : right;
		y = top < bottom ? top : bottom;
		width = std::abs(left - right);
		height = std::abs(top - bottom);
	}
}

void RectangularGraphics::OnLeftClickInternal()
{
	m_xOnClick = x;
	m_yOnClick = y;
	m_widthOnClick = width;
	m_heightOnClick = height;
}

void RectangularGraphics::OnLeftUpInternal()
{
	ResetMouseStateOnObject();
}

void RectangularGraphics::OnRightDownInternal(float mouseX, float mouseY, const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio)
{
}

void RectangularGraphics::OnRightClickInternal()
{
}

void RectangularGraphics::OnRightUpInternal()
{
}

bool RectangularGraphics::IsMouseOnObjectInternal(float mouseX,
	float mouseY,
	const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
	const float& canvasLeft,
	const float& canvasTop,
	const float& pixelRatio,
	const float& resizeRatio)
{
	static float mouseX_ = 0.f, mouseY_ = 0.f;
	mouseX_ = mouseX;
	mouseY_ = mouseY;
	//std::cout << mouseX_ << " " << mouseY_ << std::endl;
	//std::cout << x << " " << y << " "  << mouseX_ << " " << mouseY_ << std::endl;
	ResetMouseStateOnObject();
	m_mouseOnTopLeftHandle = IsMouseInHandle(mouseX_, mouseY_, x, y, m_handleSize, coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio);
	if (m_mouseOnTopLeftHandle)
	{
		//std::cout << "Mouse inside top left handle" << std::endl;
		return true;
	}
	else
	{

	}
	m_mouseOnTopRightHandle = IsMouseInHandle(mouseX_, mouseY_, x + width, y, m_handleSize, coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio);
	if (m_mouseOnTopRightHandle)
	{
		//std::cout << "Mouse inside top right handle" << std::endl;
		return true;
	}
	else
	{

	}
	m_mouseOnBottomLeftHandle = IsMouseInHandle(mouseX_, mouseY_, x, y + height, m_handleSize, coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio);
	if (m_mouseOnBottomLeftHandle)
	{
		//std::cout << "Mouse inside bottom left handle" << std::endl;
		return true;
	}
	else
	{

	}
	m_mouseOnBottomRightHandle = IsMouseInHandle(mouseX_, mouseY_, x + width, y + height, m_handleSize, coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio);
	if (m_mouseOnBottomRightHandle)
	{
		//std::cout << "Mouse inside bottom right handle" << std::endl;
		return true;
	}
	else
	{

	}
	m_mouseOnObject = IsMouseInRectangle(mouseX_, mouseY_, resizeRatio);
	if (m_mouseOnObject)
	{
		//std::cout << "Mouse In Rect" << std::endl; 
		return true;
	}
	else
	{
		//std::cout << "Mouse Ouside Rect" << std::endl;
	}
	return false;
}

RectangularGraphics::~RectangularGraphics()
{
	//std::cout << "Rectangular Graphics ID " << m_id << " deleted!" << std::endl;
}

void RectangularGraphics::CalculateRectangleCoord(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
	const float& canvasLeft,
	const float& canvasTop,
	float& left,
	float& right,
	float& top,
	float& bottom,
	const float& pixelRatio,
	const float& resizeRatio)
{
	coordToCanvasConverter(x, y, left, top);
	coordToCanvasConverter(x + width, y + height, right, bottom);
	left += canvasLeft;
	right += canvasLeft;
	top += canvasTop;
	bottom += canvasTop;
}

void RectangularGraphics::CalculateHandleRectangleCoord(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
	const float& canvasLeft,
	const float& canvasTop,
	const float& handleCenterX,
	const float& handleCenterY,
	float& left,
	float& right,
	float& top,
	float& bottom,
	const float& pixelRatio,
	const float& resizeRatio)
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

void RectangularGraphics::DrawRectangleHandles(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
	const float& canvasLeft,
	const float& canvasTop,
	const float& pixelRatio,
	const float& resizeRatio,
	const Color& color)
{
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	//top left
	float TLLeft = 0.f;
	float TLRight = 0.f;
	float TLTop = 0.f;
	float TLBottom = 0.f;
	CalculateHandleRectangleCoord(coordToCanvasConverter, canvasLeft, canvasTop, this->x, this->y, TLLeft, TLRight, TLTop, TLBottom, 1.f, resizeRatio);
	drawList->AddRectFilled(ImVec2(TLLeft, TLTop), ImVec2(TLRight, TLBottom), IM_COL32(color.R, color.G, color.B, color.A));
	if (m_mouseOnTopLeftHandle)
	{
		drawList->AddRect(ImVec2(TLLeft, TLTop), ImVec2(TLRight, TLBottom), IM_COL32(255,255,255,255));
	}
	//top right
	float TRLeft = 0.f;
	float TRRight = 0.f;
	float TRTop = 0.f;
	float TRBottom = 0.f;
	CalculateHandleRectangleCoord(coordToCanvasConverter, canvasLeft, canvasTop, this->x + this->width, this->y, TRLeft, TRRight, TRTop, TRBottom, 1.f, resizeRatio);
	drawList->AddRectFilled(ImVec2(TRLeft, TRTop), ImVec2(TRRight, TRBottom), IM_COL32(color.R, color.G, color.B, color.A));
	if (m_mouseOnTopRightHandle)
	{
		drawList->AddRect(ImVec2(TRLeft, TRTop), ImVec2(TRRight, TRBottom), IM_COL32(255, 255, 255, 255));
	}
	//bottom right
	float BRLeft = 0.f;
	float BRRight = 0.f;
	float BRTop = 0.f;
	float BRBottom = 0.f;
	CalculateHandleRectangleCoord(coordToCanvasConverter, canvasLeft, canvasTop, this->x + this->width, this->y + this->height, BRLeft, BRRight, BRTop, BRBottom, 1.f, resizeRatio);
	drawList->AddRectFilled(ImVec2(BRLeft, BRTop), ImVec2(BRRight, BRBottom), IM_COL32(color.R, color.G, color.B, color.A));
	if (m_mouseOnBottomRightHandle)
	{
		drawList->AddRect(ImVec2(BRLeft, BRTop), ImVec2(BRRight, BRBottom), IM_COL32(255, 255, 255, 255));
	}
	//bottom left
	float BLLeft = 0.f;
	float BLRight = 0.f;
	float BLTop = 0.f;
	float BLBottom = 0.f;
	CalculateHandleRectangleCoord(coordToCanvasConverter, canvasLeft, canvasTop, this->x, this->y + this->height, BLLeft, BLRight, BLTop, BLBottom, 1.f, resizeRatio);
	drawList->AddRectFilled(ImVec2(BLLeft, BLTop), ImVec2(BLRight, BLBottom), IM_COL32(color.R, color.G, color.B, color.A));
	if (m_mouseOnBottomLeftHandle)
	{
		drawList->AddRect(ImVec2(BLLeft, BLTop), ImVec2(BLRight, BLBottom), IM_COL32(255, 255, 255, 255));
	}
}

void RectangularGraphics::DrawRegionRectangle(float left, float right, float top, float bottom, const float& pixelRatio, const float& resizeRatio, Color color, bool drawHighlight)
{
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	if (drawHighlight)
	{
		drawList->AddRect(ImVec2(left, top), ImVec2(right, bottom), IM_COL32(255,255,255,150), 0.f, 0, 1.5f);
	}
	float thickness = m_focused ? 1.8f : 1.f;
	drawList->AddRect(ImVec2(left, top), ImVec2(right, bottom), IM_COL32(color.R, color.G, color.B, color.A), 0.f, 0, thickness);
}

void RectangularGraphics::RenderID(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio)
{
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	float canvasX = 0.f;
	float canvasY = 0.f;
	coordToCanvasConverter(x + width / 2.f, y, canvasX, canvasY);
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
		IM_COL32(100, 100, 100, 50),
		0);
	//render text
	drawList->AddText(textPos, IM_COL32(255, 255, 255, 170), text.c_str());
}

void RectangularGraphics::Offset(float x, float y)
{
	this->x += x;
	this->y += y;
}

std::string RectangularGraphics::GetConfig()
{
	nlohmann::json rectangleConfig;
	rectangleConfig["x"] = this->x;
	rectangleConfig["y"] = this->y;
	rectangleConfig["width"] = this->width;
	rectangleConfig["height"] = this->height;
	rectangleConfig["id"] = this->m_id;
	rectangleConfig["title"] = this->title;
	rectangleConfig["renderID"] = this->renderID;
	rectangleConfig["group"] = this->m_groupIndex;
	rectangleConfig["activate"] = this->m_active;
	rectangleConfig["visible"] = this->m_visible;
	rectangleConfig["type"] = "RectangleGraphics";
	std::string rectangleConfig_str = rectangleConfig.dump();
	return rectangleConfig_str;
}

void RectangularGraphics::SetConfig(std::string& config)
{
	//assign every value from config
	try
	{
		nlohmann::json rectangleConfig = nlohmann::json::parse(config);
		rectangleConfig.at("x").get_to(this->x);
		rectangleConfig.at("y").get_to(this->y);
		rectangleConfig.at("width").get_to(this->width);
		rectangleConfig.at("height").get_to(this->height);
		rectangleConfig.at("title").get_to(this->title);
		rectangleConfig.at("id").get_to(this->m_id);
		rectangleConfig.at("renderID").get_to(this->renderID);
		rectangleConfig.at("group").get_to(this->m_groupIndex);
		rectangleConfig.at("activate").get_to(this->m_active);
		rectangleConfig.at("visible").get_to(this->m_visible);
	}
	catch (...)
	{
		std::cout << "Failed to load configuration in Rectangle object!" << std::endl;
	}
}

void RectangularGraphics::Transform(float x, float y, float scaleX, float scaleY)
{
}

bool RectangularGraphics::operator==(const GraphicalObject& obj)
{
	//check sanity
	const RectangularGraphics* right = dynamic_cast<const RectangularGraphics*>(&obj);
	bool result = false;
	if (right != nullptr)
	{
		result = right->x == this->x &&
			right->y == this->y &&
			right->width == this->width &&
			right->height == this->height;
	}
	return result;
}

RectangularGraphics& RectangularGraphics::operator=(const RectangularGraphics& obj)
{
	//check sanity
	if (&obj != this)
	{
		const RectangularGraphics* right = dynamic_cast<const RectangularGraphics*>(&obj);
		if (right != nullptr)
		{
			this->x = right->x;
			this->y = right->y;
			this->width = right->width;
			this->height = right->height;
		}
		else
		{
			//throw std::exception("Rectangular graphics are being assigned to different type!");
		}
	}
	return *this;
}

void RectangularGraphics::RenderWhenFocused(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio)
{
	//calculate position of rectangular
	float leftCanvas = 0.f;
	float rightCanvas = 0.f;
	float bottomCanvas = 0.f;
	float topCanvas = 0.f;
	Color color = GetGlowingColor();
	CalculateRectangleCoord(coordToCanvasConverter, canvasLeft, canvasTop, leftCanvas, rightCanvas, bottomCanvas, topCanvas, pixelRatio, resizeRatio);
	DrawRegionRectangle(leftCanvas, rightCanvas, bottomCanvas, topCanvas, pixelRatio, resizeRatio, color);
	//DrawRectangleHandles(coordToCanvasConverter, canvasLeft, canvasTop, m_focusColor);
	if (renderID)
	{
		RenderID(coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio);
	}
}

void RectangularGraphics::RenderWhenControlled(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio)
{
	//calculate position of rectangular
	float leftCanvas = 0.f;
	float rightCanvas = 0.f;
	float bottomCanvas = 0.f;
	float topCanvas = 0.f;
	Color color = GetGlowingColor();
	CalculateRectangleCoord(coordToCanvasConverter, canvasLeft, canvasTop, leftCanvas, rightCanvas, bottomCanvas, topCanvas, pixelRatio, resizeRatio);
	DrawRegionRectangle(leftCanvas, rightCanvas, bottomCanvas, topCanvas, pixelRatio, resizeRatio, color, true);
	DrawRectangleHandles(coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio, color);
	if (renderID)
	{
		RenderID(coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio);
	}
}

void RectangularGraphics::RenderWhenMouseHoveredOver(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio)
{
	//calculate position of rectangular
	float leftCanvas = 0.f;
	float rightCanvas = 0.f;
	float bottomCanvas = 0.f;
	float topCanvas = 0.f;
	CalculateRectangleCoord(coordToCanvasConverter, canvasLeft, canvasTop, leftCanvas, rightCanvas, bottomCanvas, topCanvas, pixelRatio, resizeRatio);
	DrawRegionRectangle(leftCanvas, rightCanvas, bottomCanvas, topCanvas, pixelRatio, resizeRatio, m_hoverOverColor, true);
	DrawRectangleHandles(coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio, m_hoverOverColor);
	if (renderID)
	{
		RenderID(coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio);
	}
}

void RectangularGraphics::RenderSimple(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio)
{
	//calculate position of rectangular
	float leftCanvas = 0.f;
	float rightCanvas = 0.f;
	float bottomCanvas = 0.f;
	float topCanvas = 0.f;
	CalculateRectangleCoord(coordToCanvasConverter, canvasLeft, canvasTop, leftCanvas, rightCanvas, bottomCanvas, topCanvas, pixelRatio, resizeRatio);
    DrawRegionRectangle(leftCanvas, rightCanvas, bottomCanvas, topCanvas, pixelRatio, resizeRatio, m_color, false);
	if (renderID)
	{
		RenderID(coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio);
	}
}

bool RectangularGraphics::IsMouseInHandle(float mouseX,
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

bool RectangularGraphics::IsMouseInRectangle(float mouseX, float mouseY, const float& resizeRatio)
{
	return (x <= mouseX && x + width >= mouseX && y <= mouseY && y + height >= mouseY);
}

void RectangularGraphics::ResetMouseStateOnObject()
{
	m_mouseOnBottomLeftHandle = false;
	m_mouseOnTopRightHandle = false;
	m_mouseOnBottomLeftHandle = false;
	m_mouseOnBottomRightHandle = false;
	m_mouseOnObject = false;
}

void RectangularGraphics::SetGeometryOnWholeImage(int imageWidth, int imageHeight)
{
	x = 0.f;
	y = 0.f;
	width = (float)(imageWidth);
	height = (float)(imageHeight);
}


