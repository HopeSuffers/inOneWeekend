#ifndef INTERVAL_H
#define INTERVAL_H

class Interval
{
public:
    double min, max;

    Interval() : min(+RT_INFINITY), max(-RT_INFINITY)
    {} // Default Interval is empty

    Interval(double min, double max) : min(min), max(max)
    {}

    double Size() const
    {
        return max - min;
    }

    bool Contains(double x) const
    {
        return min <= x && x <= max;
    }

    bool Surrounds(double x) const
    {
        return min < x && x < max;
    }

    double Clamp(double x) const
    {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    static const Interval empty, universe;
};

const Interval Interval::empty = Interval(+RT_INFINITY, -RT_INFINITY);
const Interval Interval::universe = Interval(-RT_INFINITY, +RT_INFINITY);

#endif