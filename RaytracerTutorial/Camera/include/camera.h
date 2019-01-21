#pragma once

#include "../../Math/include/Vec3.h"
#include "../../Math/include/Utilities.h"
#include "../../Raytracer/include/Ray.h"

class Camera
{
public:
    Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 vup, float vfov, float aspect, float aperture, float focus_dist)
    {
        mLensRadius = aperture / 2;

        float theta = vfov * Utilities::PI() / 180.f;
        float half_height = tan(theta / 2);
        float half_width = aspect * half_height;

        mOrigin = lookFrom;
        
        mW = unit_vector(lookFrom - lookAt);
        mU = unit_vector(cross(vup, mW));
        mV = cross(mW, mU);

        //mLowerLeftCorner = Vec3(-half_width, -half_height, -1.f);
        mLowerLeftCorner = mOrigin - half_width * focus_dist * mU - half_height * focus_dist * mV - focus_dist * mW;
        mHorizontal = 2 * half_width * focus_dist * mU;
        mVertical = 2 * half_height * focus_dist * mV;
    }

    Ray get_ray(float s, float t)
    {
        Vec3 rd = mLensRadius * Utilities::random_in_unit_disk();
        Vec3 offset = mU * rd.x() + mV * rd.y();
        return Ray(mOrigin + offset, mLowerLeftCorner + s*mHorizontal + t*mVertical - mOrigin - offset);
    }

private:
    Vec3 mOrigin;
    Vec3 mLowerLeftCorner;
    Vec3 mHorizontal;
    Vec3 mVertical;
    Vec3 mU, mV, mW;
    float mLensRadius;
};