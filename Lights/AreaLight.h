//
// Created by Göksu Güvendiren on 19/04/2017.
//

#pragma once

#include "Light.h"

class AreaLight : public Light
{
    glm::vec3 position;
    glm::vec3 edgeVec1;
    glm::vec3 edgeVec2;
    glm::vec3 intensity;

    int id;
public:
    glm::vec3 Position() const;
    glm::vec3 Intensity(const glm::vec3& hitPoint) const;
};