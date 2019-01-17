#pragma once

#include "../../Math/include/Vec3.h"
#include "../../Raytracer/include/Ray.h"

struct hit_record
{
    float t;
    Vec3 p;
    Vec3 normal;
};

class Hitable
{
public:
    virtual bool hit(const Ray& r, float t_min, float t_max, hit_record &rec) const = 0;
};