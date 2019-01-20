#pragma once

#include <algorithm>
#include "material.h"
#include "../../Math/include/utilities.h"

class Metal : public Material
{
public:
    Metal(const Vec3 &albedo, float fuzz) : 
        mAlbedo(albedo), 
        mFuzz(std::min(1.f, fuzz)) {}

    virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const
    {
        Vec3 reflected = Utilities::reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = Ray(rec.p, reflected + mFuzz * Utilities::random_in_unit_sphere());
        attenuation = mAlbedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

private:
    Vec3 mAlbedo;
    float mFuzz;
};