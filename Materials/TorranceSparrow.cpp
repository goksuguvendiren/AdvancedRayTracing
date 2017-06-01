//
// Created by Göksu Güvendiren on 05/13/2017.
//

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>
#include "TorranceSparrow.hpp"
#include "../HitInfo.h"
#include "../Lights/Light.h"
#include "ClassicMaterial.hpp"

float D(float cos_theta, float exponent)
{
    float term_1 = (exponent + 2.f) / (2.f * glm::pi<float>());
    float term_2 = glm::pow(cos_theta, exponent);
    
    return term_1 * term_2;
}

float G(const glm::vec3& w_i, const glm::vec3& w_o, const glm::vec3& normal, float cos_theta_h, float cos_alpha_h, float cos_theta_i)
{
    float cos_theta_o = glm::dot(w_o, normal);
    
    auto local_min = std::min(1.f, (2.f * cos_theta_h * cos_theta_o) / (cos_alpha_h));
    return std::min(local_min, (2.f * cos_theta_h * cos_theta_i) / (cos_alpha_h));
}

glm::vec3 F(float cos_alpha_h, const glm::vec3& specular)
{
    auto holder = glm::vec3{1.f - specular.r, 1.f - specular.g, 1.f - specular.b};
    return specular + (holder * std::pow((1.f - cos_alpha_h), 5.f));
}

glm::vec3 TorranceSparrow::ComputeReflectance(const HitInfo& hit, const Light& light, const ClassicMaterial& material) const
{
    auto direction = light.Direction(hit.Position());
    glm::vec3 w_i  = glm::normalize(direction);
    glm::vec3 w_o  = glm::normalize(-hit.HitRay().Direction());
    glm::vec3 half = glm::normalize(w_i + w_o);
    
    float cos_alpha_h = glm::dot(half, w_o);
    float cos_theta_h = glm::dot(half, hit.Normal());
    float cos_theta_i = glm::dot(hit.Normal(), w_i);
    float cos_theta_o = glm::dot(w_o, hit.Normal());
    
    auto val = D(cos_theta_h, exponent) * G(w_i, w_o, hit.Normal(), cos_theta_h, cos_alpha_h, cos_theta_i) * F(cos_alpha_h, material.Specular()) * cos_theta_i;
    return val / (4.f * cos_theta_i * cos_theta_o);
//    
//    glm::vec3 color = diff_term + spec_term;
//    
//    return color * theta_i;
}
