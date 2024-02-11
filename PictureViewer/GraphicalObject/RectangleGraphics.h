#ifndef RECTANGULARGRAPHICS_H
#define RECTANGULARGRAPHICS_H

#include "GraphicalObject.h"
#include <vector>
#include <functional>

//really basic interface for graphical elements
class RectangularGraphics : public GraphicalObject
{
public:
	RectangularGraphics();
	RectangularGraphics(float x, float y, float width, float height, Color color, float thickness=1.f);
	RectangularGraphics(const RectangularGraphics& obj);
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
	float x;
	float y;
	float width;
	float height;
	bool IsMouseInHandle(float mouseX, float mouseY, float handleCenterX, float handleCenterY, float handleSize, const std::function<void(float, float, float&, float&)>& coordToCanvasConverter, const float& canvasLeft, const float& canvasTop, const float& pixelRatio, const float& resizeRatio);
	bool IsMouseInRectangle(float mouseX, float mouseY, const float& resizeRatio);
	void ResetMouseStateOnObject();
    void SetGeometryOnWholeImage(int imageWidth, int imageHeight) override;
	void Transform(float x, float y, float scaleX, float scaleY) override;
	~RectangularGraphics() override;
    bool operator==(const GraphicalObject& obj);
    RectangularGraphics& operator=(const RectangularGraphics& obj);
private:
	bool m_mouseOnTopLeftHandle;
	bool m_mouseOnTopRightHandle;
	bool m_mouseOnBottomLeftHandle;
	bool m_mouseOnBottomRightHandle;
	bool m_mouseOnObject;
	float m_xOnClick;
	float m_yOnClick;
	float m_widthOnClick;
	float m_heightOnClick;
	//Rectangle coordinate on canvas
	void CalculateRectangleCoord(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
		const float& canvasLeft,
		const float& canvasTop,
		float& left,
		float& right,
		float& top,
		float& bottom,
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
		const float& pixelRatio,
		const float& resizeRatio);
	void DrawRectangleHandles(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
		const float& canvasLeft,
		const float& canvasTop,
		const float& pixelRatio,
		const float& resizeRatio,
		const Color& color);
	void DrawRegionRectangle(float left,
		float right,
		float top,
		float bottom,
		const float& pixelRatio,
		const float& resizeRatio,
		Color color,
		bool drawHighlight = false);
    void RenderID(const std::function<void(float, float, float&, float&)>& coordToCanvasConverter,
		const float& canvasLeft,
		const float& canvasTop,
		const float& pixelRatio,
		const float& resizeRatio) override;
};

#endif //RECTANGULARGRAPHICS_H