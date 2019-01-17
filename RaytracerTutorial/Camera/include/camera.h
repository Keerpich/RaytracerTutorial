#pragma once

#include "../../Math/include/Vec3.h"
#include "../../Raytracer/include/Ray.h"

class Camera
{
public:
    Camera() :
        mLowerLeftCorner(-2.f, -1.f, -1.f),
        mHorizontal(4.f, 0.f, 0.f),
        mVertical(0.f, 2.f, 0.f),
        mOrigin(0.f, 0.f, 0.f)
    {}

    Ray get_ray(float u, float v)
    {
        return Ray(mOrigin, mLowerLeftCorner + u*mHorizontal + v*mVertical - mOrigin);
    }

private:
    Vec3 mOrigin;
    Vec3 mLowerLeftCorner;
    Vec3 mHorizontal;
    Vec3 mVertical;
};