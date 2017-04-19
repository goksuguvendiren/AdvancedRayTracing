//
// Created by Göksu Güvendiren on 17/04/2017.
//

#pragma once

#include "../glm/vec3.hpp"
#include "../tinyxml/tinyxml2.h"
#include <vector>
#include <memory>

class Light
{
public:
    virtual glm::vec3 Position() const = 0;
    virtual glm::vec3 Intensity(const glm::vec3& hitPoint) const = 0;
};

std::vector<std::unique_ptr<Light>> LoadLights(tinyxml2::XMLElement *elem);