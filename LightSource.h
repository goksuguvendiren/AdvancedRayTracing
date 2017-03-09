//
// Created by Göksu Güvendiren on 09/03/2017.
//

#pragma once

#include <vector>
#include "glm/vec3.hpp"
#include "tinyxml/tinyxml2.h"

// Point light source

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
    auto Intensity() const { return intensity; }
};

std::vector<LightSource> CreateLights(tinyxml2::XMLElement* elem);