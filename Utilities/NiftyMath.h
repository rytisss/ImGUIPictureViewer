#ifndef NIFTYMATH_H
#define NIFTYMATH_H

namespace NiftyMath
{
	class Point
	{
	public:
		Point();
		static void Rotate(const float& x, const float& y, const float& pivot_x, const float& pivot_y, const float& angle, float& rot_x, float& rot_y);
		~Point();
	};
}

#endif //NIFTYMATH_H
