//
// Created by Göksu Güvendiren on 08/06/2017.
//

#pragma once

#include "Materialx.hpp"

class Mirror : public Materialx
{
    int id;
    float roughness;
    glm::vec3 mirror;

public:
    Mirror(int i, const glm::vec3& m, float r) : id(i), mirror(m), roughness(r) {}

    bool is_BRDF() const  { return false; }
    int ID() const { return id; }

    glm::vec3 ComputeReflectance(const HitInfo &hit, const Light &light) const;
    glm::vec3 CalculateColor(const HitInfo &hit, int recursionDepth) const;
};