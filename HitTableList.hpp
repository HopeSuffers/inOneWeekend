#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "HitTable.hpp"
#include "RTweekend.hpp"

#include <vector>

class HitTableList : public HitTable
{
public:
    std::vector<shared_ptr<HitTable>> objects;

    HitTableList()
    {}

    HitTableList(shared_ptr<HitTable> object)
    { Add(object); }

    void Clear()
    { objects.clear(); }

    void Add(shared_ptr<HitTable> object)
    {
        objects.push_back(object);
    }

    bool Hit(const Ray& r, Interval rayT, HitRecord& rec) const override
    {
        HitRecord tempRec;
        bool hitAnything = false;
        auto closestSoFar = rayT.max;

        for (const auto &object: objects)
        {
            if (object->Hit(r, Interval(rayT.min, closestSoFar), tempRec))
            {
                hitAnything = true;
                closestSoFar = tempRec.t;
                rec = tempRec;
            }
        }

        return hitAnything;
    }
};

#endif