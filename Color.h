//
// Created by Göksu Güvendiren on 24/02/2017.
//

#ifndef RAYTRACER_COLOR_H
#define RAYTRACER_COLOR_H

#include <algorithm>
#include <boost/algorithm/clamp.hpp>

class Color
{
    int r;
    int g;
    int b;

public:
    Color() : r(0), g(0), b(0) {}
    Color(int red, int gre, int blu) : r(red), g(gre), b(blu) {}
    void Clamp();

    int R() const { return r; }
    int G() const { return g; }
    int B() const { return b; }
};

inline void Color::Clamp()
{
    r = boost::algorithm::clamp(r, 0, 255);
    g = boost::algorithm::clamp(g, 0, 255);
    b = boost::algorithm::clamp(b, 0, 255);
}

#endif //RAYTRACER_COLOR_H
