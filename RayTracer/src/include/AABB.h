#ifndef AABB_H
#define AABB_H

#include "rtweekend.h"

class AABB {
public:
    AABB() {}
    AABB(const Point3& a, const Point3& b) { minimum = a; maximum = b; }

    Point3 min() const { return minimum; }
    Point3 max() const { return maximum; }

    bool hit(const Ray& r, double t_min, double t_max) const {
        for (int a = 0; a < 3; a++) {
            auto t0 = fmin((minimum[a] - r.origin()[a]) / r.direction()[a],
                (maximum[a] - r.origin()[a]) / r.direction()[a]);
            auto t1 = fmax((minimum[a] - r.origin()[a]) / r.direction()[a],
                (maximum[a] - r.origin()[a]) / r.direction()[a]);
            t_min = fmax(t0, t_min);
            t_max = fmin(t1, t_max);
            if (t_max <= t_min)
                return false;
        }
        return true;
    }

    double area() const {
        auto a = maximum.x() - minimum.x();
        auto b = maximum.y() - minimum.y();
        auto c = maximum.z() - minimum.z();
        return 2 * (a * b + b * c + c * a);
    }

    int longest_axis() const {
        auto a = maximum.x() - minimum.x();
        auto b = maximum.y() - minimum.y();
        auto c = maximum.z() - minimum.z();
        if (a > b && a > c)
            return 0;
        else if (b > c)
            return 1;
        else
            return 2;
    }

public:
    Point3 minimum;
    Point3 maximum;
};

AABB surrounding_box(AABB box0, AABB box1) {
    Vec3 small(fmin(box0.min().x(), box1.min().x()),
        fmin(box0.min().y(), box1.min().y()),
        fmin(box0.min().z(), box1.min().z()));

    Vec3 big(fmax(box0.max().x(), box1.max().x()),
        fmax(box0.max().y(), box1.max().y()),
        fmax(box0.max().z(), box1.max().z()));

    return AABB(small, big);
}


#endif