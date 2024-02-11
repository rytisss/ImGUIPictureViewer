//
// Created by Rytis Augustauskas on 2023-03-02.
//

#include "Processing2D.h"

bool Processing2D::PointToPolygon(const Point2D& pt, const std::vector<Point2D>&points)
{
    float minX = points[0].x;
    float maxX = points[0].x;
    float minY = points[0].y;
    float maxY = points[0].y;
    for (size_t i = 1; i < points.size(); i++)
    {
        Point2D q = points[i];
        minX = std::min(q.x, minX);
        maxX = std::max(q.x, maxX);
        minY = std::min(q.y, minY);
        maxY = std::max(q.y, maxY);
    }

    if (pt.x < minX || pt.x > maxX || pt.y < minY || pt.y > maxY)
    {
        return false;
    }

    // https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html
    bool inside = false;
    for (size_t i = 0, j = points.size() - 1; i < points.size(); j = i++)
    {
        if ((points[i].y > pt.y) != (points[j].y > pt.y) &&
            pt.x < (points[j].x - points[i].x) * (pt.y - points[i].y) / (points[j].y - points[i].y) + points[i].x)
        {
            inside = !inside;
        }
    }

    return inside;
}
