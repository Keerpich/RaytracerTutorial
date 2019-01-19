#pragma once

#include <memory>

#include "../../Math/include/Vec3.h"
#include "../../Raytracer/include/Ray.h"

class Material;

struct hit_record
{
    float t;
    Vec3 p;
    Vec3 normal;
    std::shared_ptr<Material> mat_ptr;
};

class Hitable
{
public:
    virtual bool hit(const Ray& r, float t_min, float t_max, hit_record &rec) const = 0;
};