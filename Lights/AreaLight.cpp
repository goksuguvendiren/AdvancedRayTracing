//
// Created by Göksu Güvendiren on 19/04/2017.
//

#include "AreaLight.h"

std::mt19937 seed;

float generateRandomFloat()
{
    std::uniform_real_distribution<float> asd(0, 1);
    return asd(seed);
}


glm::vec3 AreaLight::Position() const
{
    auto up    = generateRandomFloat();
    auto right = generateRandomFloat();

    return position + up * edgeVec1 + right * edgeVec2;
}

glm::vec3 AreaLight::Direction(const glm::vec3& hitpos) const
{
    return Position() - hitpos;
}

glm::vec3 AreaLight::Intensity(const glm::vec3& lightPos_hitPos) const
{
    auto lensquared = [](const glm::vec3& n)
    {
        return n.r * n.r + n.g * n.g + n.b * n.b;
    };

    return intensity / lensquared(lightPos_hitPos);
}