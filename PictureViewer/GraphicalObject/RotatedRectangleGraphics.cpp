#include "RotatedRectangleGraphics.h"
#include "imgui.h"
#include <iostream>
#include <numbers>
#include "Processing2D.h"

RotatedRectangleGraphics::RotatedRectangleGraphics()
    : centerPt{0.f, 0.f}
    , width(0.f)
    , height(0.f)
    , angle(0.f)
    , m_angleOnClick(0.f)
    , m_widthOnClick(0.f)
    , m_heightOnClick(0.f)
    , m_xOnClick(0.f)
    , m_yOnClick(0.f)
    , m_mouseOnBottomLeftHandle(false)
    , m_mouseOnBottomRightHandle(false)
    , m_mouseOnTopLeftHandle(false)
    , m_mouseOnTopRightHandle(false)
    , m_mouseOnRotationHandle(false)
{
}

RotatedRectangleGraphics::RotatedRectangleGraphics(const Point2D &centerPt, float width, float height, float angle, Color color, float thickness)
    : centerPt(centerPt)
    , width(width)
    , height(height)
    , angle(angle)
    , m_angleOnClick(0.f)
    , m_widthOnClick(0.f)
    , m_heightOnClick(0.f)
    , m_xOnClick(0.f)
    , m_yOnClick(0.f)
    , m_mouseOnBottomLeftHandle(false)
    , m_mouseOnBottomRightHandle(false)
    , m_mouseOnTopLeftHandle(false)
    , m_mouseOnTopRightHandle(false)
    , m_mouseOnRotationHandle(false)
{
    m_color = color;
    m_thickness = thickness;
}

RotatedRectangleGraphics::RotatedRectangleGraphics(const RotatedRectangleGraphics &obj)
    : m_angleOnClick(0.f)
    , m_widthOnClick(0.f)
    , m_heightOnClick(0.f)
    , m_xOnClick(0.f)
    , m_yOnClick(0.f)
    , m_mouseOnBottomLeftHandle(false)
    , m_mouseOnBottomRightHandle(false)
    , m_mouseOnTopLeftHandle(false)
    , m_mouseOnTopRightHandle(false)
    , m_mouseOnRotationHandle(false)
{
    this->centerPt = obj.centerPt;
    this->width = obj.width;
    this->height = obj.height;
    this->angle = obj.angle;
    this->m_color = obj.m_color;
    this->m_thickness = obj.m_thickness;
}

RotatedRectangleGraphics::~RotatedRectangleGraphics()
{
}

std::array<Point2D, 4> RotatedRectangleGraphics::GetCorners()
{
    std::array<Point2D, 4> corners;
    // go in the following order:
    // top left -> top right -> bottom right -> bottom left
    std::array<Point2D, 4> rectangleCorners;
    // top left
    rectangleCorners[0].x = centerPt.x - width / 2.f;
    rectangleCorners[0].y = centerPt.y - height / 2.f;
    // top right
    rectangleCorners[1].x = centerPt.x + width / 2.f;
    rectangleCorners[1].y = centerPt.y - height / 2.f;
    // bottom right
    rectangleCorners[2].x = centerPt.x + width / 2.f;
    rectangleCorners[2].y = centerPt.y + height / 2.f;
    // bottom left
    rectangleCorners[3].x = centerPt.x - width / 2.f;
    rectangleCorners[3].y = centerPt.y + height / 2.f;
    // rotate all points
    for (size_t i = 0; i < rectangleCorners.size(); i++)
    {
        corners[i] = this->RotatePt(centerPt, rectangleCorners[i], angle * (std::numbers::pi_v<float> / 180.f));
    }
    return corners;
}

std::string RotatedRectangleGraphics::GetConfig()
{
    nlohmann::json rotRectConfig;
    rotRectConfig["center_x"] = this->centerPt.x;
    rotRectConfig["center_y"] = this->centerPt.y;
    rotRectConfig["width"] = this->width;
    rotRectConfig["height"] = this->height;
    rotRectConfig["title"] = this->title;
    rotRectConfig["id"] = this->m_id;
    rotRectConfig["renderID"] = this->renderID;
    rotRectConfig["group"] = this->m_groupIndex;
    rotRectConfig["activate"] = this->m_active;
    rotRectConfig["visible"] = this->m_visible;
    rotRectConfig["type"] = "CircleGraphics";
    std::string rotRectConfig_str = rotRectConfig.dump();
    return rotRectConfig_str;
}
void RotatedRectangleGraphics::SetConfig(std::string &config)
{
    //assign every value from config
    try
    {
        nlohmann::json rotRectConfig = nlohmann::json::parse(config);
        rotRectConfig.at("center_x").get_to(this->centerPt.x);
        rotRectConfig.at("center_y").get_to(this->centerPt.y);
        rotRectConfig.at("width").get_to(this->width);
        rotRectConfig.at("height").get_to(this->height);
        rotRectConfig.at("angle").get_to(this->angle);
        rotRectConfig.at("title").get_to(this->title);
        rotRectConfig.at("id").get_to(this->m_id);
        rotRectConfig.at("renderID").get_to(this->renderID);
        rotRectConfig.at("group").get_to(this->m_groupIndex);
        rotRectConfig.at("activate").get_to(this->m_active);
        rotRectConfig.at("visible").get_to(this->m_visible);
    }
    catch (...)
    {
        std::cout << "Failed to load configuration in Rotated Rectangle object!" << std::endl;
    }
}
void RotatedRectangleGraphics::Init(int id)
{
    m_id = id;
}
void RotatedRectangleGraphics::RenderWhenFocused(const std::function<void(float,
                                                                          float,
                                                                          float &,
                                                                          float &)> &coordToCanvasConverter,
                                                 const float &canvasLeft,
                                                 const float &canvasTop,
                                                 const float &pixelRatio,
                                                 const float &resizeRatio)
{
    std::array<Point2D, 4> corners = CalculateRotRectCornersCoord(coordToCanvasConverter,
                                                                  canvasLeft,
                                                                  canvasTop,
                                                                  pixelRatio,
                                                                  resizeRatio);
    Color color = GetGlowingColor();
    DrawRotatedRect(corners,
                    pixelRatio,
                    resizeRatio,
        color,
                    true);
}
void RotatedRectangleGraphics::RenderWhenControlled(const std::function<void(float,
                                                                             float,
                                                                             float &,
                                                                             float &)> &coordToCanvasConverter,
                                                    const float &canvasLeft,
                                                    const float &canvasTop,
                                                    const float &pixelRatio,
                                                    const float &resizeRatio)
{   
    std::array<Point2D, 4> corners = CalculateRotRectCornersCoord(coordToCanvasConverter,
                                                                  canvasLeft,
                                                                  canvasTop,
                                                                  pixelRatio,
                                                                  resizeRatio);
    Color color = GetGlowingColor();
    DrawRotatedRect(corners,
                    pixelRatio,
                    resizeRatio,
                    color,
                    true,
                    2.f);
}
void RotatedRectangleGraphics::RenderWhenMouseHoveredOver(const std::function<void(float,
                                                                                   float,
                                                                                   float &,
                                                                                   float &)> &coordToCanvasConverter,
                                                          const float &canvasLeft,
                                                          const float &canvasTop,
                                                          const float &pixelRatio,
                                                          const float &resizeRatio)
{
    std::array<Point2D, 4> corners = CalculateRotRectCornersCoord(coordToCanvasConverter,
                                                                  canvasLeft,
                                                                  canvasTop,
                                                                  pixelRatio,
                                                                  resizeRatio);
    DrawRotatedRect(corners,
                    pixelRatio,
                    resizeRatio,
                    m_hoverOverColor,
                    true,
                    1.5f);
}
void RotatedRectangleGraphics::RenderSimple(const std::function<void(float,
                                                                     float,
                                                                     float &,
                                                                     float &)> &coordToCanvasConverter,
                                            const float &canvasLeft,
                                            const float &canvasTop,
                                            const float &pixelRatio,
                                            const float &resizeRatio)
{
    std::array<Point2D, 4> corners = CalculateRotRectCornersCoord(coordToCanvasConverter,
                                                                      canvasLeft,
                                                                      canvasTop,
                                                                      pixelRatio,
                                                                      resizeRatio);
    DrawRotatedRect(corners,
                    pixelRatio,
                    resizeRatio,
                    m_color,
                    true);
}
void RotatedRectangleGraphics::DeInit()
{
}
void RotatedRectangleGraphics::Offset(float x, float y)
{
    this->centerPt.x += x;
    this->centerPt.y += y;
}
void RotatedRectangleGraphics::SetGeometryOnWholeImage(int imageWidth, int imageHeight)
{

}
void RotatedRectangleGraphics::Transform(float x, float y, float scaleX, float scaleY)
{

}
bool RotatedRectangleGraphics::operator==(const GraphicalObject &obj)
{
    //check sanity
    const RotatedRectangleGraphics *right = dynamic_cast<const RotatedRectangleGraphics *>(&obj);
    bool result = false;
    if (right != nullptr)
    {
        result = right->centerPt.x == this->centerPt.x &&
            right->centerPt.y == this->centerPt.y &&
            right->width == this->width &&
            right->height == this->height &&
            right->angle == this->angle;
    }
    return result;
}
RotatedRectangleGraphics &RotatedRectangleGraphics::operator=(const RotatedRectangleGraphics &obj)
{
    //check sanity
    if (&obj != this)
    {
        const RotatedRectangleGraphics *right = dynamic_cast<const RotatedRectangleGraphics *>(&obj);
        if (right != nullptr)
        {
            this->centerPt.x = right->centerPt.x;
            this->centerPt.y = right->centerPt.y;
            this->width = right->width;
            this->height = right->height;
            this->angle = right->angle;
        }
        else
        {
#if defined(_WIN32)
            throw std::exception("Rotated rectangle graphics are being assigned to different type!");
#endif
        }
    }
    return *this;
}
void RotatedRectangleGraphics::RenderID(const std::function<void(float,
                                                                 float,
                                                                 float &,
                                                                 float &)> &coordToCanvasConverter,
                                        const float &canvasLeft,
                                        const float &canvasTop,
                                        const float &pixelRatio,
                                        const float &resizeRatio)
{

}
void RotatedRectangleGraphics::OnLeftDownInternal(float mouseX,
                                                  float mouseY,
                                                  const std::function<void(float,
                                                                           float,
                                                                           float &,
                                                                           float &)> &coordToCanvasConverter,
                                                  const float &canvasLeft,
                                                  const float &canvasTop,
                                                  const float &pixelRatio,
                                                  const float &resizeRatio)
{

}
void RotatedRectangleGraphics::OnLeftClickInternal()
{

}
void RotatedRectangleGraphics::OnLeftUpInternal()
{

}
void RotatedRectangleGraphics::OnRightDownInternal(float mouseX,
                                                   float mouseY,
                                                   const std::function<void(float,
                                                                            float,
                                                                            float &,
                                                                            float &)> &coordToCanvasConverter,
                                                   const float &canvasLeft,
                                                   const float &canvasTop,
                                                   const float &pixelRatio,
                                                   const float &resizeRatio)
{

}
void RotatedRectangleGraphics::OnRightClickInternal()
{

}
void RotatedRectangleGraphics::OnRightUpInternal()
{

}
bool RotatedRectangleGraphics::IsMouseOnObjectInternal(float mouseX,
                                                       float mouseY,
                                                       const std::function<void(float,
                                                                                float,
                                                                                float &,
                                                                                float &)> &coordToCanvasConverter,
                                                       const float &canvasLeft,
                                                       const float &canvasTop,
                                                       const float &pixelRatio,
                                                       const float &resizeRatio)
{
    std::array<Point2D, 4> corners = GetCorners();
    return Processing2D::PointToPolygon(Point2D{mouseX, mouseY},
                                        std::vector<Point2D>(corners.begin(), corners.end()));
}

Point2D RotatedRectangleGraphics::RotatePt(const Point2D &originPt, const Point2D &pt, float angle)
{
    float sinAngle = std::sin(angle);
    float cosAngle = std::cos(angle);
    Point2D rotPt;
    rotPt.x = originPt.x + cosAngle * (pt.x - originPt.x) - sinAngle * (pt.y - originPt.y);
    rotPt.y = originPt.y + sinAngle * (pt.x - originPt.x) + cosAngle * (pt.y - originPt.y);
    return rotPt;
}

std::array<Point2D, 4> RotatedRectangleGraphics::CalculateRotRectCornersCoord(
    const std::function<void(float, float, float &, float &)> &coordToCanvasConverter,
    const float &canvasLeft,
    const float &canvasTop,
    const float &pixelRatio,
    const float &resizeRatio)
{
    // this will be points in the canvas
    std::array<Point2D, 4> canvasCorners;
    // get corner points
    std::array<Point2D, 4> corners = GetCorners();
    for (size_t i = 0; i < corners.size(); i++)
    {
        coordToCanvasConverter(corners[i].x, corners[i].y, canvasCorners[i].x, canvasCorners[i].y);
        canvasCorners[i].x += canvasLeft;
        canvasCorners[i].y += canvasTop;
    }
    return canvasCorners;
}

void RotatedRectangleGraphics::DrawRotatedRect(const std::array<Point2D, 4> &cornersInCanvas,
                                               const float &pixelRatio,
                                               const float &resizeRatio,
                                               Color color,
                                               bool highlight,
                                               float thickness)
{
    ImDrawList *drawList = ImGui::GetWindowDrawList();
    for (size_t i = 0; i < cornersInCanvas.size(); i++)
    {
        size_t i_2 = (i + 1) % cornersInCanvas.size(); // second point
        drawList->AddLine(ImVec2(cornersInCanvas[i].x, cornersInCanvas[i].y),
                          ImVec2(cornersInCanvas[i_2].x, cornersInCanvas[i_2].y),
                          IM_COL32((int)color.R, (int)color.G, (int)color.B, (int)color.A),
                          thickness);
    }
}