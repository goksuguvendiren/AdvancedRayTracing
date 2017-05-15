//
// Created by Göksu Güvendiren on 05/13/2017.
//

#pragma once

#include <glm/glm.hpp>
#include "BRDF.hpp"

class HitInfo;
class Light;
class ClassicMaterial;

class BlinnPhongModified : public BRDF
{
    int   id;
    float exponent;
    bool  normalized;
    
public:
    BlinnPhongModified(int pid, float exp, bool norm) : id(pid), exponent(exp), normalized(norm) {}
    int ID() const { return id; }
    glm::vec3 ComputeReflectance(const HitInfo& hit, const Light& light, const ClassicMaterial& material) const;
};
