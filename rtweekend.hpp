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
inline double degrees_to_radians(double degrees)
{
    return degrees * PI / 180.0;
}

inline double random_double()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max)
{
    static std::mt19937 generator;  // Shared generator (optional to share)
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

// Common Headers
#include "interval.hpp"
#include "vec3.hpp"
#include "color.hpp"
#include "ray.hpp"

#endif