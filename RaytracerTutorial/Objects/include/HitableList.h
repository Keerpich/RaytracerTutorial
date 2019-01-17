#pragma once

#include "hitable.h"

#include <list>
#include <memory>

class HitableList : public Hitable
{
public:
    using HitableSharedPtr = std::shared_ptr<Hitable>;
    using HitableSharedPtrList = std::list<HitableSharedPtr>;

    HitableList() {}
    HitableList(HitableSharedPtrList l) : list(l) {}
    virtual bool hit(const Ray& r, float t_min, float t_max, hit_record &rec) const;

private:
    HitableSharedPtrList list;
};

bool HitableList::hit(const Ray& r, float t_min, float t_max, hit_record &rec) const
{
    hit_record temp_rec;
    bool hit_anything = false;
    float closest_so_far = t_max;

    for(const HitableSharedPtr& ptr_hitable : list)
    {
        if (ptr_hitable->hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}