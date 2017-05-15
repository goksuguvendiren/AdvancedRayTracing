//
// Created by Göksu Güvendiren on 05/13/2017.
//

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>
#include "PhongBRDF_Mdf.hpp"
#include "../HitInfo.h"
#include "../Lights/Light.h"
#include "ClassicMaterial.hpp"

glm::vec3 PhongModified::ComputeReflectance(const HitInfo& hit, const Light& light, const ClassicMaterial& material) const
{
    glm::vec3 w_i = glm::normalize(light.Direction(hit.Position()));
    glm::vec3 rn_wi = glm::normalize(2 * (glm::dot(hit.Normal(), w_i)) * hit.Normal() - w_i);
    
    glm::vec3 w_o = glm::normalize(-hit.HitRay().Direction());
    
    float alpha = std::max(0.f, glm::dot(rn_wi, w_o));
    float theta_i = glm::dot(hit.Normal(), w_i);
    
    glm::vec3 color = material.Diffuse() / glm::pi<float>();
    color += (material.Specular() * std::pow(alpha, exponent));
    
    return color * theta_i; // theta_i in the rendering equation
}
