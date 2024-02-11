#ifndef POLYGONGRAPHICS_H
#define POLYGONGRAPHICS_H

#include "GraphicalObject.h"
#include <vector>
#include <functional>
#include "Point2D.h"
#include "Polygon.h"

using namespace Geometries;

//really basic interface for graphical elements
class PolygonGraphics : public GraphicalObject
{
public:
	PolygonGraphics();
	PolygonGraphics(Geometries::Polygon& polygon, Color color, float thickness = 1.f);
	PolygonGraphics(const PolygonGraphics& obj);
    std::string GetConfig() override;
    void SetConfig(std::string& config) override;
    void Init(int id) override;
    void DeInit() override;
    bool IsMouseOnObjectInternal(float mouseX, float mouseY, const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio) override;
    void OnLeftDownInternal(float mouseX, float mouseY, const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio) override;
	void OnLeftClickInternal() override;
    void OnLeftUpInternal() override;
    void OnRightDownInternal(float mouseX, float mouseY, const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio) override;
    void OnRightClickInternal() override;
    void OnRightUpInternal() override;
    void Offset(float x, float y) override;
    void RenderWhenFocused(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio) override;
    void RenderWhenControlled(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio) override;
    void RenderWhenMouseHoveredOver(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio) override;
    void RenderSimple(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio) override;
	bool IsMouseInHandle(float mouseX, float mouseY, float handleCenterX, float handleCenterY, float handleSize, const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio);
	bool IsMouseInPolygon(float mouseX, float mouseY,
		const float& resizeRatio);
	void AddPoint(Point2D pt);
	void RemovePoint(int index);
	Geometries::Polygon polygon;
	void ResetMousePressedStateOnObject();
	void ResetMouseHoveredStateOnObject();
    void SetGeometryOnWholeImage(int imageWidth, int imageHeight) override;
    void Transform(float x, float y, float scaleX = 1.f, float scaleY = 1.f) override;
	~PolygonGraphics() override;
    bool operator==(const GraphicalObject& obj);
    PolygonGraphics& operator=(const PolygonGraphics& obj);
private:
	Geometries::Polygon m_polygonOnMouseDown;
	std::vector<bool> m_polygonCornerHovered;
	std::vector<bool> m_polygonCornerPressed;
	bool m_mousePressedOnPolygon;
	bool m_mouseHoveredOnPolygon;
	//inside or on handle
	void CalculateHandleRectangleCoord(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
		const float& canvasLeft,
		const float& canvasTop,
		const float& handleCenterX,
		const float& handleCenterY,
		float& left,
		float& right,
		float& top,
		float& bottom,
		const float& pixelRatio,
		const float& resizeRatio);
	void CalculatePointCoord(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
		const float& canvasLeft,
		const float& canvasTop,
		const float& ptX,
		const float& ptY,
		float& ptXCanvas,
		float& ptYCanvas);
	void DrawRectangleHandles(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
		const float& canvasLeft,
		const float& canvasTop,
		const float& pixelRatio,
		const float& resizeRatio,
		const Color& color);
	void DrawRegionPolygon(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
		const float& canvasLeft,
		const float& canvasTop,
		const float& pixelRatio,
		const float& resizeRatio,
		Color color,
		bool drawHighlight = false,
		float thickness = 1.f);
    void RenderID(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
		const float& canvasLeft,
		const float& canvasTop,
		const float& pixelRatio,
		const float& resizeRatio) override;
};

#endif //POLYGONGRAPHICS_H