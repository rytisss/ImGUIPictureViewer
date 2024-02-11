#ifndef PROCESSING2D_H
#define PROCESSING2D_H

#include <vector>
#include "Point2D.h"

using namespace Geometries;

class Processing2D
{
 public:
    Processing2D() = delete;
    static bool PointToPolygon(const Point2D& pt, const std::vector<Point2D>& points);
    ~Processing2D() = delete;;
};

#endif //PROCESSING2D_H
