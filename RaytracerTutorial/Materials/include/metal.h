#pragma once

#include "material.h"
#include "../../Math/include/utilities.h"

class Metal : public Material
{
public:
    Metal(const Vec3 &a) : mAlbedo(a) {}

    virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const
    {
        Vec3 reflected = Utilities::reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = Ray(rec.p, reflected);
        attenuation = mAlbedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

private:
    Vec3 mAlbedo;
};