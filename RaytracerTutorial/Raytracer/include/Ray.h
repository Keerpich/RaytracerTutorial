#pragma once

#include "Math/include/Vec3.h"

class Ray
{
public:
	Ray() = default;
	Ray(const Vec3 &origin, const Vec3 &direction) :
		mOrigin(origin), mDirection(direction) {}

	~Ray() = default;

	inline Vec3 origin() const { return mOrigin; }
	inline Vec3 direction() const { return mDirection; }
	inline Vec3 point_at_parameter(float t) const { return mOrigin + t * mDirection; }

private:
	Vec3 mOrigin;
	Vec3 mDirection;
};