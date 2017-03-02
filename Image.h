//
// Created by Göksu Güvendiren on 24/02/2017.
//

#ifndef RAYTRACER_IMAGE_H
#define RAYTRACER_IMAGE_H

#include <vector>

#include "Color.h"

class Image
{
    std::vector<Color> pixels;

    unsigned int width;
    unsigned int height;

public :
    Image() : width(0), height(0) {}
    Image(unsigned int w, unsigned int h) : width(w), height(h) { pixels.resize(width * height); }

    Color& at(int i, int j) { return pixels[i * width + j]; }
    Color at(int i, int j) const { return pixels[i * width + j]; }

    unsigned int Width() const { return width; }
    unsigned int Height() const { return height; }

    void Print();
};

#endif //RAYTRACER_IMAGE_H
