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
    Ray ray;

    const Material* mat;

public:
    HitInfo(glm::vec3 n,
            const Material& m,
            glm::vec3 point,
            Ray r,
            float t = std::numeric_limits<float>::infinity()) : normal(n), mat(&m), position(point), ray(r), param(t)
    {}

    HitInfo() : param(std::numeric_limits<float>::infinity()) {}

    const Material& Material() const { return *mat; }
    float Parameter() const { return param; }
    glm::vec3 Normal() const { return normal; }
    glm::vec3 Position() const { return position; }
    Ray HitRay() const { return ray; }
};