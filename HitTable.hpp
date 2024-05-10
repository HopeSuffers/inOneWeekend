#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.hpp"

class Material;

class HitRecord
{
public:
    Point3 p;
    Vec3 normal;
    shared_ptr<Material> mat;
    double t;
    bool frontFace;

    void SetFaceNormal(const Ray &r, const Vec3 &outwardNormal)
    {
        // Sets the Hit record normal vector.
        frontFace = Dot(r.direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class HitTable
{
public:
    ~HitTable() = default;

    virtual bool Hit(const Ray& r, Interval rayT, HitRecord& rec) const = 0;
};

#endif