#pragma once

#include "material.h"
#include "../../Math/include/Utilities.h"

//This is also the diffuse material
class Lambertian : public Material
{
public:
    Lambertian(const Vec3& a) : mAlbedo(a) {}

    virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const
    {
        Vec3 target = rec.p + rec.normal + Utilities::random_in_unit_sphere();
        scattered = Ray(rec.p, target-rec.p);
        attenuation = mAlbedo;
        return true;
    }

private:
    Vec3 mAlbedo;
};