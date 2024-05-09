#ifndef INTERVAL_H
#define INTERVAL_H

#include "rtweekend.hpp"

class interval {
public:
    double min, max;

    interval() : min(+RT_INFINITY), max(-RT_INFINITY) {} // Default interval is empty

    interval(double min, double max) : min(min), max(max) {}

    double size() const {
        return max - min;
    }

    bool contains(double x) const {
        return min <= x && x <= max;
    }

    bool surrounds(double x) const {
        return min < x && x < max;
    }

    static const interval empty, universe;
};

const interval interval::empty    = interval(+RT_INFINITY, -RT_INFINITY);
const interval interval::universe = interval(-RT_INFINITY, +RT_INFINITY);

#endif