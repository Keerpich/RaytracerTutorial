#pragma once

#include "Vec3.h"
#include <random>

class Utilities
{
public:
    static Vec3 random_in_unit_sphere()
    {
        //init random
        std::random_device random_device;
        std::default_random_engine random_engine(random_device());
        std::uniform_real_distribution<> random_distribution;

        Vec3 p;

        do
        {
            float rand_x = static_cast<float>(random_distribution(random_engine));
            float rand_y = static_cast<float>(random_distribution(random_engine));
            float rand_z = static_cast<float>(random_distribution(random_engine));

            p = 2.f * Vec3(rand_x, rand_y, rand_z) - Vec3(1.f, 1.f, 1.f);
        } while(p.squared_length() >= 1.f);

        return p;
    }

    static Vec3 reflect(const Vec3& v, const Vec3& n)
    {
        return v - 2 * dot(v, n) * n;
    }
};