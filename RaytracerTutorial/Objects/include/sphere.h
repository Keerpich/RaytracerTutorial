#pragma once

#include "hitable.h"
#include "../../Materials/include/material.h"

class Sphere : public Hitable
{
public:
    Sphere() {};
    Sphere(Vec3 cen, float r, std::shared_ptr<Material> mat) : 
        center(cen), 
        radius(r),
        mpMaterial(mat)
    {};
    
    //from Hitable
    virtual bool hit(const Ray &r, float t_min, float t_max, hit_record &rec) const;

private:
    Vec3 center;
    float radius;
    std::shared_ptr<Material> mpMaterial;
};

bool Sphere::hit(const Ray &r, float t_min, float t_max, hit_record &rec) const
{
    rec.mat_ptr = mpMaterial;

    Vec3 oc = r.origin() - center;
    
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius*radius;

    float discriminant = b * b - a * c;

    if (discriminant > 0)
    {
        float temp = (-b - sqrt(b*b - a*c)) / a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(temp);
            rec.normal = (rec.p - center) / radius;
            return true;
        }

        temp = (-b + sqrt(b * b - a * c)) / a;

        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(temp);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
    }

    return false;
}