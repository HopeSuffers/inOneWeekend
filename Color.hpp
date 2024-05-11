#ifndef COLOR_H
#define COLOR_H

#include <sstream>

#include "Interval.hpp"

using Color = Vec3;

inline double LinearToGamma(double linearComponent)
{
    if (linearComponent > 0)
    {
        return sqrt(linearComponent);
    }

    return 0;
}

std::string WriteColor(const Color &pixelColor)
{
    auto r = pixelColor.X();
    auto g = pixelColor.Y();
    auto b = pixelColor.Z();

    // Apply a linear to gamma transform for gamma 2
    r = LinearToGamma(r);
    g = LinearToGamma(g);
    b = LinearToGamma(b);

    // Translate the [0,1] component values to the byte range [0,255].
    static const Interval Intensity(0.0, 0.999);
    int rByte = int(255.999 * r);
    int gByte = int(255.999 * g);
    int bByte = int(255.999 * b);

    // Create an output string stream
    std::ostringstream oss;

    // Use the output string stream to format your string
    oss << rByte << ' ' << gByte << ' ' << bByte << '\n';

    // Get the string from the output string stream
    return oss.str();
}

#endif