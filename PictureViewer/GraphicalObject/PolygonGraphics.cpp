#include "PolygonGraphics.h"
#include <cstdlib>
#include "imgui.h"
#include <iostream>
#include "Processing2D.h"
#include "NiftyMath.h"

PolygonGraphics::PolygonGraphics()
    : m_mousePressedOnPolygon(false)
    , m_mouseHoveredOnPolygon(false)
{
}

PolygonGraphics::PolygonGraphics(Geometries::Polygon& polygon, Color color, float thickness)
	: m_mousePressedOnPolygon(false)
	, m_mouseHoveredOnPolygon(false)
{
	this->polygon = polygon;
	this->m_color = color;
	this->m_thickness = thickness;
}

PolygonGraphics::PolygonGraphics(const PolygonGraphics& obj)
{
    polygon = obj.polygon;
    m_mousePressedOnPolygon = obj.m_mousePressedOnPolygon;
    m_mouseHoveredOnPolygon = obj.m_mouseHoveredOnPolygon;
	this->m_color = obj.m_color;
	this->m_thickness = obj.m_thickness;
}

void PolygonGraphics::Init(int id)
{
	m_id = id;
}

void PolygonGraphics::DeInit()
{
}

void PolygonGraphics::OnLeftDownInternal(float mouseX, float mouseY, const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio)
{
	float diffX = mouseX - m_mouseXOnLeftClick;
	float diffY = mouseY - m_mouseYOnLeftClick;
	//for (size_t i = 0; i < polygon.points.size(); i++)
	//{
	//	if (m_polygonCornerPressed[i] || m_mousePressedOnPolygon)
	//	{
    //        polygon.points[i].x = m_polygonOnMouseDown.points[i].x + diffX;
    //        polygon.points[i].y = m_polygonOnMouseDown.points[i].y + diffY;
	//	}
	//}
}

void PolygonGraphics::OnLeftClickInternal()
{
	//for (size_t i = 0; i < polygon.points.size(); i++)
	//{
    //    m_polygonOnMouseDown.points[i].x = polygon.points[i].x;
    //    m_polygonOnMouseDown.points[i].y = polygon.points[i].y;
	//	m_polygonCornerPressed[i] = m_polygonCornerHovered[i];
	//}
	m_mousePressedOnPolygon = m_mouseHoveredOnPolygon;
}

void PolygonGraphics::OnLeftUpInternal()
{
	ResetMousePressedStateOnObject();
	m_mousePressedOnPolygon = false;
}

void PolygonGraphics::OnRightDownInternal(float mouseX, float mouseY, const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio)
{
}

void PolygonGraphics::OnRightClickInternal()
{
}

void PolygonGraphics::OnRightUpInternal()
{
}

bool PolygonGraphics::IsMouseOnObjectInternal(float mouseX, float mouseY, const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio)
{
	ResetMouseHoveredStateOnObject();
	//check if something is inside handles
	for (size_t i = 0; i < m_polygonOnMouseDown.points.size(); i++)
	{
		bool clickedInsideCorner = IsMouseInHandle(mouseX,
                                                   mouseY,
                                                   m_polygonOnMouseDown.points[i].x,
                                                   m_polygonOnMouseDown.points[i].y,
                                                   m_handleSize,
                                                   coordToCanvasConverter,
                                                   canvasLeft,
                                                   canvasTop,
                                                   pixelRatio,
                                                   resizeRatio);
		if (clickedInsideCorner)
		{
			m_polygonCornerHovered[i] = true;
			m_mouseHoveredOnPolygon = false;
			return true;
		}
	}
	//check if click is inside region
	bool clickInsidePolygon = IsMouseInPolygon(mouseX, mouseY, resizeRatio);
	if (clickInsidePolygon)
	{
		m_mouseHoveredOnPolygon = true;
		return true;
	}
	m_mouseHoveredOnPolygon = false;
	return false;
}

void PolygonGraphics::CalculateHandleRectangleCoord(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
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
	float halfHandleSize = m_handleSize /2.f;
	float centerX = 0.f;
	float centerY = 0.f;
	coordToCanvasConverter(handleCenterX, handleCenterY, centerX, centerY);
	left = canvasLeft + centerX - halfHandleSize;
	right = canvasLeft + centerX + halfHandleSize;
	top = canvasTop + centerY - halfHandleSize;
	bottom = canvasTop + centerY + halfHandleSize;
}

void PolygonGraphics::CalculatePointCoord(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
	const float& canvasLeft,
	const float& canvasTop,
	const float& ptX,
	const float& ptY,
	float& ptXCanvas,
	float& ptYCanvas)
{
	coordToCanvasConverter(ptX, ptY, ptXCanvas, ptYCanvas);
	ptXCanvas += canvasLeft;
	ptYCanvas += canvasTop;
}

void PolygonGraphics::DrawRectangleHandles(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
	const float& canvasLeft,
	const float& canvasTop,
	const float& pixelRatio,
	const float& resizeRatio,
	const Color& color)
{
	//ImDrawList* drawList = ImGui::GetWindowDrawList();
	//float handleLeft = 0.f;
	//float handleRight = 0.f;
	//float handleTop = 0.f;
	//float handleBottom = 0.f;
	//for (size_t i = 0; i < polygon.points.size(); i++)
	//{
	//	CalculateHandleRectangleCoord(coordToCanvasConverter, canvasLeft, canvasTop, polygon.points[i].x, polygon.points[i].y, handleLeft, handleRight, handleTop, handleBottom, pixelRatio, resizeRatio);
	//	drawList->AddRectFilled(ImVec2(handleLeft, handleTop), ImVec2(handleRight, handleBottom), IM_COL32(color.R, color.G, color.B, color.A));
	//	if (m_polygonCornerPressed[i] || m_polygonCornerHovered[i])
	//	{
	//		drawList->AddRect(ImVec2(handleLeft, handleTop), ImVec2(handleRight, handleBottom), IM_COL32(255, 255, 255, 255));
	//	}
	//}
}

void PolygonGraphics::DrawRegionPolygon(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
	const float& canvasLeft,
	const float& canvasTop,
	const float& pixelRatio,
	const float& resizeRatio,
	Color color,
	bool drawHighlight,
	float thickness)
{
	if (polygon.points.size() < 2)
	{
		return;
	}
#ifndef NDEBUG
	if (polygon.points.size() > 60000)
	{
		std::cout << "Polygon contains more than 60000 points, consider approximating polygon!" << std::endl;
	}
#endif

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	int pointCount = polygon.points.size();
	float pt1XCanvas = 0.f;
	float pt1YCanvas = 0.f;
	float pt2XCanvas = 0.f;
	float pt2YCanvas = 0.f;
	std::vector<ImVec2> m_polygonImVec2;
	m_polygonImVec2.resize(polygon.points.size());

	for (int i = 0; i < pointCount; i++)
	{
		CalculatePointCoord(coordToCanvasConverter, canvasLeft, canvasTop, polygon.points[i].x, polygon.points[i].y, m_polygonImVec2[i].x, m_polygonImVec2[i].y);
	}

	float thickness_ = drawHighlight ? thickness + 1.5f : thickness;
	ImU32 color_ = drawHighlight ? IM_COL32(255, 255, 255, 255) : IM_COL32(color.R, color.G, color.B, color.A);
	// lines approach
	for (size_t i = 0; i < m_polygonImVec2.size() - 1; i++)
	{
		drawList->AddLine(m_polygonImVec2[i], m_polygonImVec2[i + 1], color_, thickness_);
	}
	// first to last point
	drawList->AddLine(m_polygonImVec2[0], m_polygonImVec2[m_polygonImVec2.size() - 1], color_, thickness_);

	//drawList->AddPolyline(m_polygonImVec2.data(), m_polygonImVec2.size(), color_, ImDrawFlags_Closed, thickness_); // asserts on really big polygon
}

void PolygonGraphics::RenderID(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio)
{
}

void PolygonGraphics::Offset(float x, float y)
{
	for (size_t i = 0; i < polygon.points.size(); i++)
	{
        polygon.points[i].x += x;
        polygon.points[i].y += y;
	}
}

std::string PolygonGraphics::GetConfig()
{
	nlohmann::json polygonConfig;
	nlohmann::json polygonCorner;
	for (size_t i = 0; i < polygon.points.size(); i++)
	{
		nlohmann::json ptConfig;
		ptConfig["x"] = polygon.points[i].x;
		ptConfig["y"] = polygon.points[i].y;
		polygonCorner[std::to_string(i)] = ptConfig;
	}
	polygonConfig["polygonCorners"] = polygonCorner;
	polygonConfig["id"] = this->m_id;
	polygonConfig["title"] = this->title;
	polygonConfig["renderID"] = this->renderID;
	polygonConfig["type"] = "PolygonGraphics";
	polygonConfig["group"] = this->m_groupIndex;
	polygonConfig["activate"] = this->m_active;
	polygonConfig["visible"] = this->m_visible;
	std::string polygonConfig_str = polygonConfig.dump();
	return polygonConfig_str;
}

void PolygonGraphics::SetConfig(std::string& config)
{
	//assign every value from config
	try
	{
		nlohmann::json polygonConfig = nlohmann::json::parse(config);
		nlohmann::json polygonCornerConfig = polygonConfig["polygonCorners"];
        polygon.points.clear();
		for (auto& polygonCorner : polygonCornerConfig.items())
		{
			nlohmann::json polygonCornerJSON = nlohmann::json::parse(polygonCorner.value().dump());
			Point2D pt;
			polygonCornerJSON.at("x").get_to(pt.x);
			polygonCornerJSON.at("y").get_to(pt.y);
            polygon.points.push_back(pt);
			m_polygonOnMouseDown.points.push_back(pt);
			m_polygonCornerPressed.push_back(false);
			m_polygonCornerHovered.push_back(false);
		}
		polygonConfig.at("title").get_to(this->title);
		polygonConfig.at("id").get_to(this->m_id);
		polygonConfig.at("renderID").get_to(this->renderID);
		polygonConfig.at("group").get_to(this->m_groupIndex);
		polygonConfig.at("activate").get_to(this->m_active);
		polygonConfig.at("visible").get_to(this->m_visible);
	}
	catch (...)
	{
		std::cout << "Failed to load configuration in Polygon object!" << std::endl;
	}
}

void PolygonGraphics::Transform(float x, float y, float scaleX, float scaleY)
{
}

bool PolygonGraphics::operator==(const GraphicalObject& obj)
{
	// check sanity
	const PolygonGraphics* other = dynamic_cast<const PolygonGraphics*>(&obj);
	bool result = false;
	if (other != nullptr)
    {
		if (other->polygon.points.size() == polygon.points.size())
		{
			result = true;
			for (size_t i = 0; i < polygon.points.size(); i++)
			{
				if (polygon.points[i].x != other->polygon.points[i].x ||
                    polygon.points[i].y != other->polygon.points[i].y)
				{
					result = false;
					break;
				}
			}
		}
	}
	return result;
}

PolygonGraphics& PolygonGraphics::operator=(const PolygonGraphics& obj)
{
	if (&obj != this)
	{
		const PolygonGraphics* other = dynamic_cast<const PolygonGraphics*>(&obj);
		if (other != nullptr)
		{
			this->polygon.points.clear();
            this->polygon.points = other->polygon.points;
		}
	}
	return *this;
}

void PolygonGraphics::SetGeometryOnWholeImage(int imageWidth, int imageHeight)
{
	if (imageWidth <= 0 || imageHeight <= 0)
	{
		std::cout << "Image dimension is 0, can't perform points reorder!" << std::endl;
		return;
	}
	int pointCount = polygon.points.size();
	if (pointCount == 0)
	{
		return; //nothing to reorder
	}
	//try to put even number of points on the image sides
	if (pointCount % 4 == 0)
	{
		int oneForthOfPoints = pointCount / 4;
		float stepAlongWidth = (float)imageWidth / (float)oneForthOfPoints;
		float stepAlongHeight = (float)imageHeight / (float)oneForthOfPoints;
		//top
		for (int i = 0; i < oneForthOfPoints; i++)
		{
			int pointId = i;
			polygon.points[pointId].x = (float)i * stepAlongWidth;
            polygon.points[pointId].y = 0.f;
		}
		//right
		for (int i = 0; i < oneForthOfPoints; i++)
		{
			int pointId = i + oneForthOfPoints;
            polygon.points[pointId].x = (float)imageWidth;
            polygon.points[pointId].y = (float)i * stepAlongHeight;
		}
		//bottom
		for (int i = 0; i < oneForthOfPoints; i++)
		{
			int pointId = i + oneForthOfPoints * 2;
            polygon.points[pointId].x = (float)imageWidth - (float)i * stepAlongWidth;
            polygon.points[pointId].y = (float)imageHeight;
		}
		//left
		for (int i = 0; i < oneForthOfPoints; i++)
		{
			int pointId = i + oneForthOfPoints * 3;
            polygon.points[pointId].x = 0.f;
            polygon.points[pointId].y = (float)imageHeight - (float)i * stepAlongHeight;
		}
	}
	else
	{
		//throw std::exception("Implement order condition when geometry point count is not dividing by 4!");
	}
}

void PolygonGraphics::RenderWhenFocused(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio)
{
	//calculate position of rectangular
	float leftCanvas = 0.f;
	float rightCanvas = 0.f;
	float bottomCanvas = 0.f;
	float topCanvas = 0.f;
	Color color = GetGlowingColor();
	DrawRegionPolygon(coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio, color, false, m_thickness);
	//DrawRectangleHandles(coordToCanvasConverter, canvasLeft, canvasTop, m_focusColor);
}

void PolygonGraphics::RenderWhenControlled(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio)
{
	//calculate position of rectangular
	float leftCanvas = 0.f;
	float rightCanvas = 0.f;
	float bottomCanvas = 0.f;
	float topCanvas = 0.f;
	Color color = GetGlowingColor();
	DrawRegionPolygon(coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio, color, true, m_thickness);
	DrawRectangleHandles(coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio, color);
}

void PolygonGraphics::RenderWhenMouseHoveredOver(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio)
{
	//calculate position of rectangular
	float leftCanvas = 0.f;
	float rightCanvas = 0.f;
	float bottomCanvas = 0.f;
	float topCanvas = 0.f;
	DrawRegionPolygon(coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio, m_hoverOverColor, true, m_thickness);
	DrawRectangleHandles(coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio, m_hoverOverColor);
}

void PolygonGraphics::RenderSimple(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio)
{
	//calculate position of rectangular
	float leftCanvas = 0.f;
	float rightCanvas = 0.f;
	float bottomCanvas = 0.f;
	float topCanvas = 0.f;
	DrawRegionPolygon(coordToCanvasConverter, canvasLeft, canvasTop, pixelRatio, resizeRatio, m_color, false, m_thickness);
}

bool PolygonGraphics::IsMouseInHandle(float mouseX,
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
	return (left <= mouseX && right >= mouseX && bottom <= mouseY && top >= mouseY);
}

bool PolygonGraphics::IsMouseInPolygon(float mouseX, float mouseY, const float& resizeRatio)
{
	return Processing2D::PointToPolygon(Point2D{mouseX, mouseY}, polygon.points);
}

void PolygonGraphics::AddPoint(Point2D pt)
{
	polygon.points.push_back(pt);
	m_polygonOnMouseDown.points.push_back(pt); //TODO
	m_polygonCornerPressed.push_back(false);
	m_polygonCornerHovered.push_back(false);
}

void PolygonGraphics::RemovePoint(int index)
{
	if (index > 0 && polygon.points.size() < index)
	{
		polygon.points.erase(polygon.points.begin() + index);
		m_polygonOnMouseDown.points.erase(m_polygonOnMouseDown.points.begin() + index);
		m_polygonCornerPressed.erase(m_polygonCornerPressed.begin() + index);
		m_polygonCornerHovered.erase(m_polygonCornerHovered.begin() + index);
	}
}

void PolygonGraphics::ResetMousePressedStateOnObject()
{
	for (size_t i = 0; i < m_polygonCornerPressed.size(); i++)
	{
		m_polygonCornerPressed[i] = false;
	}
}

void PolygonGraphics::ResetMouseHoveredStateOnObject()
{
	for (size_t i = 0; i < m_polygonCornerHovered.size(); i++)
	{
		m_polygonCornerHovered[i] = false;
	}
}

PolygonGraphics::~PolygonGraphics()
{
	//std::cout << "Polygon Graphics ID " << m_id << " deleted!" << std::endl;
}


