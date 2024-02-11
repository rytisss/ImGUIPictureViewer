#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include "Point2D.h"

namespace Geometries
{
struct Polygon
{
    std::vector<Point2D> points;
};
}

#endif //POLYGON_H