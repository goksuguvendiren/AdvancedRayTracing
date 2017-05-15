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
    
    glm::vec3 diff_term = material.Diffuse() / glm::pi<float>();
    glm::vec3 spec_term = material.Specular() * std::pow(alpha, exponent);
    
    if (normalized)
    {
        spec_term *= (exponent + 2.f) / 2.f;
        spec_term /= glm::pi<float>();
    }
    
    glm::vec3 color = diff_term + spec_term;
    
    return color * theta_i; // theta_i in the rendering equation
}
