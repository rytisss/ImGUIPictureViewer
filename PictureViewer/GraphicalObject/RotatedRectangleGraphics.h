#ifndef ROTATEDRECTANGLEGRAPHICS_H
#define ROTATEDRECTANGLEGRAPHICS_H

#include <array>
#include "GraphicalObject.h"
#include "Point2D.h"

using namespace Geometries;

class RotatedRectangleGraphics : public GraphicalObject
{
 public:
    RotatedRectangleGraphics();
    RotatedRectangleGraphics(const Point2D& centerPt, float width, float height, float angle, Color color, float thickness = 1.f);
    RotatedRectangleGraphics(const RotatedRectangleGraphics &obj);
    ~RotatedRectangleGraphics();
    Point2D centerPt;
    float width;
    float height;
    float angle;
    std::array<Point2D, 4> GetCorners();
    std::string GetConfig() override;
    void SetConfig(std::string &config) override;
    void Init(int id) override;
    void RenderWhenFocused(const std::function<void(float, float, float &, float &)> &coordToCanvasConverter,
                           const float &canvasLeft,
                           const float &canvasTop,
                           const float &pixelRatio,
                           const float &resizeRatio) override;
    void RenderWhenControlled(const std::function<void(float, float, float &, float &)> &coordToCanvasConverter,
                              const float &canvasLeft,
                              const float &canvasTop,
                              const float &pixelRatio,
                              const float &resizeRatio) override;
    void RenderWhenMouseHoveredOver(const std::function<void(float, float, float &, float &)> &coordToCanvasConverter,
                                    const float &canvasLeft,
                                    const float &canvasTop,
                                    const float &pixelRatio,
                                    const float &resizeRatio) override;
    void RenderSimple(const std::function<void(float, float, float &, float &)> &coordToCanvasConverter,
                      const float &canvasLeft,
                      const float &canvasTop,
                      const float &pixelRatio,
                      const float &resizeRatio) override;
    void DeInit() override;
    void Offset(float x, float y) override;
    void SetGeometryOnWholeImage(int imageWidth, int imageHeight) override;
    void Transform(float x, float y, float scaleX, float scaleY) override;
    bool operator==(const GraphicalObject &obj);
    RotatedRectangleGraphics &operator=(const RotatedRectangleGraphics &obj);
 protected:
    void RenderID(const std::function<void(float, float, float &, float &)> &coordToCanvasConverter,
                  const float &canvasLeft,
                  const float &canvasTop,
                  const float &pixelRatio,
                  const float &resizeRatio) override;
    void OnLeftDownInternal(float mouseX,
                            float mouseY,
                            const std::function<void(float, float, float &, float &)> &coordToCanvasConverter,
                            const float &canvasLeft,
                            const float &canvasTop,
                            const float &pixelRatio,
                            const float &resizeRatio) override;
    void OnLeftClickInternal() override;
    void OnLeftUpInternal() override;
    void OnRightDownInternal(float mouseX,
                             float mouseY,
                             const std::function<void(float, float, float &, float &)> &coordToCanvasConverter,
                             const float &canvasLeft,
                             const float &canvasTop,
                             const float &pixelRatio,
                             const float &resizeRatio) override;
    void OnRightClickInternal() override;
    void OnRightUpInternal() override;
    bool IsMouseOnObjectInternal(float mouseX,
                                 float mouseY,
                                 const std::function<void(float, float, float &, float &)> &coordToCanvasConverter,
                                 const float &canvasLeft,
                                 const float &canvasTop,
                                 const float &pixelRatio,
                                 const float &resizeRatio) override;

 private:
    bool m_mouseOnTopLeftHandle;
    bool m_mouseOnTopRightHandle;
    bool m_mouseOnBottomLeftHandle;
    bool m_mouseOnBottomRightHandle;
    bool m_mouseOnRotationHandle;
    bool m_mouseOnObject;
    float m_xOnClick;
    float m_yOnClick;
    float m_widthOnClick;
    float m_heightOnClick;
    float m_angleOnClick;
    // helper function to define the corners of rotated rectangle [this is same function from ImageProcessing/Points.h]
    // Rotate point
    Point2D RotatePt(const Point2D &originPt, const Point2D &pt, float angle);
    // Get rotated rectangle coordinated in the canvas
    std::array<Point2D, 4> CalculateRotRectCornersCoord(
        const std::function<void(float, float, float &, float &)> &coordToCanvasConverter,
        const float &canvasLeft,
        const float &canvasTop,
        const float &pixelRatio,
        const float &resizeRatio);

    void DrawRotatedRect(const std::array<Point2D, 4> &cornersInCanvas,
                         const float &pixelRatio,
                         const float &resizeRatio,
                         Color color,
                         bool highlight,
                         float thickness=1.f);
};

#endif //ROTATEDRECTANGLEGRAPHICS_H
