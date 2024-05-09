#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <numbers>

// Constants
#define RT_INFINITY std::numeric_limits<double>::infinity()
#define PI std::numbers::pi

// Common Headers
#include "interval.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "vec3.hpp"

// C++ Std Usings
using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Utility Functions
inline double degrees_to_radians(double degrees)
{
    return degrees * PI / 180.0;
}

#endif