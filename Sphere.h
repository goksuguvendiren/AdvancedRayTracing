//
// Created by Göksu Güvendiren on 24/02/2017.
//

#pragma once

#include <vector>
#include "glm/vec3.hpp"
#include "Material.h"

class Ray;
class HitInfo;

class Sphere
{
    float radius;
    glm::vec3 center;
    int materialID;

public:
    Sphere(float rd = 1.0f, glm::vec3 c = {1.0f, 0.0f, 0.0f}, int id = 1) : radius(rd), center(c), materialID(id) {}
    std::pair<bool, HitInfo> Hit (const Ray &ray) const;
    bool BoolHit (const Ray &ray) const;
};

std::vector<Sphere> CreateSpheres(tinyxml2::XMLElement* elem);