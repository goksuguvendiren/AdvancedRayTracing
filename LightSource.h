//
// Created by Göksu Güvendiren on 09/03/2017.
//

#pragma once

#include <vector>
#include <cmath>
#include "glm/vec3.hpp"
#include "tinyxml/tinyxml2.h"

// Point light source

inline float len(const glm::vec3& n)
{
    return std::sqrt(n.r * n.r + n.g * n.g + n.b * n.b);
}

class LightSource
{
    glm::vec3 position;
    glm::vec3 intensity;

    int id;

public:
    LightSource(int lid = 1,
                glm::vec3 pos = {0, 0, 0},
                glm::vec3 intens = {0, 0, 0}) : id(lid),
                                                position(pos),
                                                intensity(intens) {}

    auto Position()  const { return position; }
    auto Intensity(const glm::vec3& dist) const { return intensity / std::powf(len(dist), 2); }
};

std::vector<LightSource> CreateLights(tinyxml2::XMLElement* elem);