//
// Created by Goksu Guvendiren on 24/04/2017.
//

#include "DirectionalLight.hpp"



glm::vec3 DirectionalLight::Intensity(const glm::vec3& lightPos_hitPos) const
{
    return radiance;
}

glm::vec3 DirectionalLight::Direction(const glm::vec3& hitpos) const
{
    return -direction;
}
