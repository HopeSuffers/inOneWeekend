#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <random>
#include <iostream>
#include <limits>
#include <memory>
#include <numbers>

// Constants
#define RT_INFINITY std::numeric_limits<double>::infinity()
#define PI std::numbers::pi

// C++ Std Usings
using std::fabs;
using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Utility Functions
inline double DegreesToRadians(double degrees)
{
    return degrees * PI / 180.0;
}

inline double RandomDouble()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double RandomDouble(double min, double max)
{
    static std::mt19937 generator;
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

// Common Headers
#include "Interval.hpp"
#include "Vec3.hpp"
#include "Color.hpp"
#include "Ray.hpp"

#endif