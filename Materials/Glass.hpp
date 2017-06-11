//
// Created by Göksu Güvendiren on 11/06/2017.
//

#pragma once

#include "Materialx.hpp"

class Glass : public Materialx
{
    int id;
    float refraction_index;
    glm::vec3 transparency;

public:
    Glass(int i, float r_i, const glm::vec3& tr) : id(i), refraction_index(r_i), transparency(tr) {}

    bool is_BRDF() const  { return false; }
    int ID() const { return id; }

    glm::vec3 ComputeReflectance(const HitInfo &hit, const Light &light) const;
    glm::vec3 CalculateColor(const HitInfo &hit, int recursionDepth) const;
};
