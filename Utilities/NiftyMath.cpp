#include "NiftyMath.h"
#include <cmath>

#define PI 3.1415926535f
namespace NiftyMath
{
	Point::Point()
	{
	}

	void Point::Rotate(const float& x, const float& y, const float& pivot_x, const float& pivot_y, const float& angle, float& rot_x, float& rot_y)
	{
		float angle_rad = angle * PI / 180.f;
		float proj_y = std::sin(angle_rad);
		float proj_x = std::cos(angle_rad);
		//translate
		float norm_x = x - pivot_x;
		float norm_y = y - pivot_y;
		//rotate and translate back
		rot_x = norm_x * proj_x - norm_y * proj_y + pivot_x;
		rot_y = norm_x * proj_y + norm_y * proj_x + pivot_y;
	}

	Point::~Point()
	{
	}
}
