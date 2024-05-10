#ifndef SPHERE_H
#define SPHERE_H

#include "HitTable.hpp"
#include "RTweekend.hpp"

class Sphere : public HitTable
{
public:
    Sphere(const Point3& center, double radius, shared_ptr<Material> mat)
            : center(center), radius(fmax(0,radius)), mat(mat) {}


    bool Hit(const Ray &r, Interval rayT, HitRecord &rec) const override
    {
        Vec3 oc = center - r.origin();
        auto a = r.direction().LengthSquared();
        auto h = Dot(r.direction(), oc);
        auto c = oc.LengthSquared() - radius * radius;

        auto discriminant = h * h - a * c;
        if (discriminant < 0)
        {
            return false;
        }

        auto sqrtD = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (h - sqrtD) / a;
        if (!rayT.Surrounds(root))
        {
            root = (h + sqrtD) / a;
            if (root <= rayT.min || rayT.max <= root)
            {
                return false;
            }
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        Vec3 outwardNormal = (rec.p - center) / radius;
        rec.SetFaceNormal(r, outwardNormal);
        rec.mat = mat;

        return true;
    }

private:
    Point3 center;
    double radius;
    shared_ptr<Material> mat;
};

#endif