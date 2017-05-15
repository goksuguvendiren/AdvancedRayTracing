//
// Created by Göksu Güvendiren on 05/13/2017.
//

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>
#include "BlinnPhongBRDF_Mdf.hpp"
#include "../HitInfo.h"
#include "../Lights/Light.h"
#include "ClassicMaterial.hpp"

glm::vec3 BlinnPhongModified::ComputeReflectance(const HitInfo& hit, const Light& light, const ClassicMaterial& material) const
{
    auto direction = light.Direction(hit.Position());
    glm::vec3 w_i  = glm::normalize(direction);
    glm::vec3 w_o  = glm::normalize(-hit.HitRay().Direction());
    glm::vec3 half = glm::normalize(w_i + w_o);
    
    float alpha   = std::max(0.f, glm::dot(half, hit.Normal()));
    float theta_i = glm::dot(hit.Normal(), w_i);
    
    glm::vec3 color = material.Diffuse() / glm::pi<float>();
    color += (material.Specular() * std::pow(alpha, exponent));
    
    return color * theta_i;
}
