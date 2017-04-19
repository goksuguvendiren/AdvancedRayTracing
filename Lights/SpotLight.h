//
// Created by Göksu Güvendiren on 17/04/2017.
//

#pragma once

#include <cmath>
#include <iostream>

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

    glm::vec3 Intensity(const glm::vec3& hitPoint) const
    {
        auto lensquared = [](const glm::vec3& n)
        {
            return n.r * n.r + n.g * n.g + n.b * n.b;
        };

        auto dir = glm::normalize(hitPoint - position);
        float cosAlpha = glm::dot(dir, glm::normalize(direction));
        if (cosAlpha < 0) return {0, 0, 0};
        float alpha = std::acos(cosAlpha);

        if (alpha > coverageAngle)
            return {0, 0, 0};

        else if (alpha > fallOffAngle)
            return intensity / lensquared(dir);

        else {
            float cosBeta = glm::cos(coverageAngle);
            float cosTheta = glm::cos(fallOffAngle - alpha);

            float a = (cosAlpha - cosBeta) / (cosTheta - cosBeta);
            return (intensity / lensquared(dir)) * float(glm::pow(a, 4));
        }
    }
};