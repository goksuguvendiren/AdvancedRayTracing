//
// Created by Göksu Güvendiren on 03/03/2017.
//

#pragma once

#include <cassert>
#include "glm/vec3.hpp"
#include "Ray.h"
#include "Material.h"

class HitInfo
{
    float param;
    glm::vec3 normal;
    glm::vec3 position;

    Material mat;
//    Ray ray;


public:
    HitInfo(glm::vec3 n,
            Material m,
//            Ray r = {{0, 0, 0}, {}}) : normal(n), mat(m), param(t), ray(r)
            glm::vec3 point,
            float t = std::numeric_limits<float>::infinity()) : normal(n), mat(m), position(point), param(t)
    {}

    const Material& Material() const { return mat; }
    float Parameter() const { return param; }
    glm::vec3 Normal() const { return normal; }
//    Ray Ray() const { return ray; }
//    glm::vec3 Position() const { return ray.Origin() + param * ray.Direction(); }
    glm::vec3 Position() const { return position; }
};