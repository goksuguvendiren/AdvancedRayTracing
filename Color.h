//
// Created by Göksu Güvendiren on 24/02/2017.
//

#ifndef RAYTRACER_COLOR_H
#define RAYTRACER_COLOR_H

#include <algorithm>
#include <boost/algorithm/clamp.hpp>

class Color
{
    float r;
    float g;
    float b;
    float a;

public:
    Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
    Color(float red, float gre, float blu, float alpha = 1.0f) : r(red), g(gre), b(blu), a(alpha) {}
    void Clamp();

    float R() const { return r; }
    float G() const { return g; }
    float B() const { return b; }
    float A() const { return a; }
};

inline void Color::Clamp()
{
    r = boost::algorithm::clamp(r, 0, 255.0f);
    g = boost::algorithm::clamp(g, 0, 255.0f);
    b = boost::algorithm::clamp(b, 0, 255.0f);

    a = boost::algorithm::clamp(a, 0, 1.0f);
}

#endif //RAYTRACER_COLOR_H
