//
// Created by Göksu Güvendiren on 05/13/2017.
//

#pragma once

#include <glm/glm.hpp>
#include "BRDF.hpp"

class HitInfo;
class Light;
class ClassicMaterial;

class BlinnPhong : public BRDF
{
    int id;
    float exponent;
    
public:
    BlinnPhong(int pid, float exp) : id(pid), exponent(exp) {}
    int ID() const { return id; }
    glm::vec3 ComputeReflectance(const HitInfo& hit, const Light& light, const ClassicMaterial& material) const;
};
