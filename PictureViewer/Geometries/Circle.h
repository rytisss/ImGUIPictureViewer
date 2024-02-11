#ifndef CIRCLE_H
#define CIRCLE_H

#include "Point2D.h"

namespace Geometries
{
    struct Circle
    {
        // center point
        Point2D center;
        float radius {0.f};
    };
}

#endif //CIRCLE_H