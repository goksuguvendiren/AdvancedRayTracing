//
// Created by Goksu Guvendiren on 24/04/2017.
//

#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
    glm::vec3 radiance;
    glm::vec3 direction;

    int id;

public:
    DirectionalLight(glm::vec3 dir, glm::vec3 rad, int i) : radiance(rad), direction(dir), id(i) {}
    glm::vec3 Direction(const glm::vec3& hitpos) const;
    glm::vec3 Intensity(const glm::vec3& lightPos_hitPos) const;
};
