//
// Created by Göksu Güvendiren on 17/04/2017.
//

#pragma once

#include <cmath>
#include <iostream>
#include <algorithm>

#include "../glm/geometric.hpp"
#include "../glm/vec3.hpp"
#include "../glm/glm.hpp"
#include "Light.h"

class SpotLight : public Light
{
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 intensity;

    float coverageAngle;
    float fallOffAngle;

    int id;

public:
    SpotLight(int lid, glm::vec3 pos, glm::vec3 intens,
              glm::vec3 dir, float ca, float foa) : id(lid), position(pos),
                                                    intensity(intens), direction(dir),
                                                    coverageAngle(ca), fallOffAngle(foa){}

    glm::vec3 Position() const { return position; }

    glm::vec3 Direction(const glm::vec3& hitpos) const
    {
        return position - hitpos;
    }

    glm::vec3 Intensity(const glm::vec3& lightPos_hitPos) const
    {
        auto lensquared = [](const glm::vec3& n)
        {
            return n.r * n.r + n.g * n.g + n.b * n.b;
        };

        auto dir = -lightPos_hitPos;
        float cosAlpha = glm::dot(glm::normalize(dir), glm::normalize(direction));

        float alpha = std::acos(cosAlpha) * float(180.f / M_PI);
        float a = (alpha - coverageAngle) / (fallOffAngle - coverageAngle);
        a = std::max(0.f, std::min(a, 1.f));

        return (intensity / lensquared(dir)) * float(glm::pow(a, 4));
    }
    
    
    int ID() const { return id; }
};
