#pragma once

#include "../../Math/include/Vec3.h"
#include "../../Math/include/Utilities.h"
#include "../../Raytracer/include/Ray.h"

class Camera
{
public:
    Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 vup, float vfov, float aspect)
    {
        Vec3 v, u, w;

        float theta = vfov * Utilities::PI() / 180.f;
        float half_height = tan(theta / 2);
        float half_width = aspect * half_height;

        mOrigin = lookFrom;
        
        w = unit_vector(lookFrom - lookAt);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        //mLowerLeftCorner = Vec3(-half_width, -half_height, -1.f);
        mLowerLeftCorner = mOrigin - half_width * u - half_height * v - w;
        mHorizontal = 2 * half_width * u;
        mVertical = 2 * half_height * v;
    }

    Ray get_ray(float s, float t)
    {
        return Ray(mOrigin, mLowerLeftCorner + s*mHorizontal + t*mVertical - mOrigin);
    }

private:
    Vec3 mOrigin;
    Vec3 mLowerLeftCorner;
    Vec3 mHorizontal;
    Vec3 mVertical;
};