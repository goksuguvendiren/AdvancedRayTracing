//
// Created by Göksu Güvendiren on 24/02/2017.
//

#ifndef RAYTRACER_IMAGE_H
#define RAYTRACER_IMAGE_H

#include <vector>

#include "Color.h"
#include "glm/vec3.hpp"

class Image
{
    std::vector<glm::vec3> pixels;

    unsigned int width;
    unsigned int height;

public :
    Image() : width(0), height(0) {}
    Image(unsigned int w, unsigned int h) : width(w), height(h) {
        pixels.resize(width * height);
    }

    glm::vec3& at(int i, int j) { return pixels[i * width + j]; }
    glm::vec3  at(int i, int j) const { return pixels[i * width + j]; }

    unsigned int Width() const { return width; }
    unsigned int Height() const { return height; }
};

#endif //RAYTRACER_IMAGE_H
