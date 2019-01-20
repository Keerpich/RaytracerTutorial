#pragma once

#include "material.h"
#include "../../Math/include/utilities.h"

class Dielectric : public Material
{
public:
    Dielectric(float ri) : mRefIdx(ri) {}

    virtual bool scatter(const Ray& r_in, const hit_record &rec, Vec3& attenuation, Ray& scattered) const
    {
        Vec3 outward_normal;
        Vec3 reflected = Utilities::reflect(r_in.direction(), rec.normal);
        float ni_over_nt;
        attenuation = Vec3(1.f, 1.f, 1.f);
        Vec3 refracted;
        float reflect_prob;
        float cosine;

        if (dot(r_in.direction(), rec.normal) > 0)
        {
            outward_normal = -rec.normal;
            ni_over_nt = mRefIdx;
            cosine = mRefIdx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        else
        {
            outward_normal = rec.normal;
            ni_over_nt = 1.f / mRefIdx;
            cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }

        if (Utilities::refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
        {
            // scattered = Ray(rec.p, refracted);
            reflect_prob = Utilities::schlick(cosine, mRefIdx);
        }
        else
        {
            // scattered = Ray(rec.p, reflected);
            reflect_prob = 1.f;
        }

        if (Utilities::random_float() <= reflect_prob)
        {
            scattered = Ray(rec.p, reflected);
        }
        else
        {
            scattered = Ray(rec.p, refracted);
        }
        
        return true;
    }

private:
    float mRefIdx;
};