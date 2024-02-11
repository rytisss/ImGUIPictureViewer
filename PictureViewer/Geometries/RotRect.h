#ifndef ROTRECT_H
#define ROTRECT_H

#include "Point2D.h"

namespace Geometries
{
struct Rect
{
    // Center point
    Point2D centerPt;
    float width {0.f};
    float height {0.f};
    // angle in degrees
    float angle {0.f};
};
}

#endif //ROTRECT_H