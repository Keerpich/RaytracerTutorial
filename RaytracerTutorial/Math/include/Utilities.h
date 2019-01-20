#pragma once

#include "Vec3.h"
#include <random>

#define _USE_MATH_DEFINES
#include <math.h>
#undef _USE_MATH_DEFINES

class Utilities
{
public:
    static Vec3 random_in_unit_sphere()
    {
        Vec3 p;

        do
        {
            float rand_x = random_float();
            float rand_y = random_float();
            float rand_z = random_float();

            p = 2.f * Vec3(rand_x, rand_y, rand_z) - Vec3(1.f, 1.f, 1.f);
        } while(p.squared_length() >= 1.f);

        return p;
    }

    static Vec3 reflect(const Vec3& v, const Vec3& n)
    {
        return v - 2 * dot(v, n) * n;
    }

    static bool refract(const Vec3& v, const Vec3& n, float ni_over_nt, Vec3& refracted)
    {
        Vec3 uv = unit_vector(v);
        float dt = dot(uv, n);
        float discriminant = 1.f - ni_over_nt * ni_over_nt * (1 - dt * dt);
        if (discriminant > 0)
        {
            refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
            return true;
        }
        else
        {
            return false;
        }
    }

    static float schlick(float cosine, float ref_idx)
    {
        float r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1- r0) * powf((1 - cosine), 5);
    }

    static float random_float()
    {
        //init random
        static std::random_device random_device;
        static std::default_random_engine random_engine(random_device());
        static std::uniform_real_distribution<> random_distribution;

        return static_cast<float>(random_distribution(random_engine));
    }

    static constexpr float PI()
    {
        return M_PI;
    }
};