#pragma once

#include "../../Raytracer/include/Ray.h"
#include "../../Objects/include/Hitable.h"
#include "../../Math/include/Vec3.h"



class Material
{
    Vec3 v;
    Ray r;
    hit_record h;

public:
    virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const = 0;
};