//
// Created by Göksu Güvendiren on 03/03/2017.
//

#pragma once

#include "glm/vec3.hpp"
#include "Ray.h"
#include "Material.h"

class HitInfo
{
    glm::vec3 normal;
    Material mat;
    Ray ray;

    float param;

public:
    HitInfo(glm::vec3 n = {0, 0, 0},
            Material m = {0, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            float t = std::numeric_limits<float>::max(),
            Ray r = {{0, 0, 0}, {}}) : normal(n), mat(m), param(t), ray(r) {}

    const Material& Material() { return mat; }
    float Parameter() { return param; }
};