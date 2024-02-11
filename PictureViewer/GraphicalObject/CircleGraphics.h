#ifndef CIRCLEGRAPHICS_H
#define CIRCLEGRAPHICS_H

#include "GraphicalObject.h"
#include <vector>
#include <functional>
#include "../Geometries/Circle.h"

using namespace Geometries;

class CircleGraphics: public GraphicalObject
{
public:
	CircleGraphics();
	CircleGraphics(const CircleGraphics& obj);
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
	Circle circle;
	bool IsMouseInResizeHandle(float mouseX, float mouseY, float handleCenterX, float handleCenterY, float handleSize, const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio);
	bool IsMouseInCircle(float mouseX, float mouseY);
	void ResetMouseStateOnObject();
	void SetGeometryOnWholeImage(int imageWidth, int imageHeight) override;
	void Transform(float x, float y, float scaleX = 1.f, float scaleY = 1.f) override;
	~CircleGraphics() override;
    bool operator==(const GraphicalObject& obj);
    CircleGraphics& operator=(const CircleGraphics& obj);
private:
	bool m_mouseOnResizeHandle;
	bool m_mouseOnObject;
	float m_xOnClick;
	float m_yOnClick;
	float m_radiusOnClick;
	//Circle center and radius on canvas
	void CalculateCircleCoord(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
		const float& canvasLeft,
		const float& canvasTop, 
		float& x_,
		float& y_,
		float& radius_,
		const float& pixelRatio, 
		const float& resizeRatio);
	void CalculateHandleRectangleCoord(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
		const float& canvasLeft,
		const float& canvasTop,
		const float& handleCenterX,
		const float& handleCenterY,
		float& left,
		float& right,
		float& top,
		float& bottom,
		const float& pixelRatio);
	void DrawCircleHandle(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
		const float& canvasLeft,
		const float& canvasTop,
		const float& pixelRatio, 
		const Color& color);
	void DrawCircle(float x,
		float y,
		float radius,
		const float& pixelRatio, 
		Color color,
		bool drawHighlight = false);
	virtual void RenderID(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
		const float& canvasLeft,
		const float& canvasTop,
		const float& pixelRatio, 
		const float& resizeRatio) override;
};

#endif //CIRCLEGRAPHICS_H