//
// Created by Göksu Güvendiren on 09/03/2017.
//

#pragma once

#include <vector>
#include <cmath>
#include "../glm/vec3.hpp"
#include "../tinyxml/tinyxml2.h"
#include "Light.h"

// Point light source
class PointLight : public Light
{
    glm::vec3 position;
    glm::vec3 intensity;

    int id;

public:
    PointLight(int lid = 1,
                glm::vec3 pos = {0, 0, 0},
                glm::vec3 intens = {0, 0, 0}) : id(lid),
                                                position(pos),
                                                intensity(intens) {}

    glm::vec3 Position() const { return position; }
    glm::vec3 Intensity(const glm::vec3& lightpos, const glm::vec3& hitPoint) const
    {
        auto lensquared = [](const glm::vec3& n)
        {
            return n.r * n.r + n.g * n.g + n.b * n.b;
        };

        return intensity / lensquared(lightpos - hitPoint);
    }
};
