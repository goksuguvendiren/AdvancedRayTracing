//
// Created by Göksu Güvendiren on 03/03/2017.
//

#pragma once

#include <cassert>
#include "glm/vec3.hpp"
#include "Ray.h"
#include "Material.h"

inline float len(const glm::vec3& n)
{
    return std::sqrt(n.r * n.r + n.g * n.g + n.b * n.b);
}

class HitInfo
{
    glm::vec3 normal;
    Material mat;
    Ray ray;

    float param;

public:
    HitInfo(glm::vec3 n = {0, 0, 0},
            Material m = {0, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            float t = std::numeric_limits<float>::infinity(),
            Ray r = {{0, 0, 0}, {}}) : normal(n), mat(m), param(t), ray(r)
    {
        assert(len(normal) <= 1.0001f);
    }

    const Material& Material() const { return mat; }
    float Parameter() const { return param; }
    glm::vec3 Normal() const { return normal; }
    Ray Ray() const { return ray; }
    glm::vec3 Position() const { return ray.Origin() + param * ray.Direction(); }
};