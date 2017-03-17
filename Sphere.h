//
// Created by Göksu Güvendiren on 24/02/2017.
//

#pragma once

#include <vector>
#include "glm/vec3.hpp"
#include "Material.h"
#include "Shape.h"

class Ray;
class HitInfo;

class Sphere : public Shape
{
    int id;
    float radius;
    glm::vec3 center;
    int materialID;

public:
    Sphere(int sid = 1, float rd = 1.0f, glm::vec3 c = {1.0f, 0.0f, 0.0f}, int mid = 1) : id(sid), radius(rd), center(c), materialID(mid) {}
    std::pair<bool, HitInfo> Hit(const Ray &ray) const;
    bool FastHit (const Ray &ray) const;
    int ID() const { return id; }
};

std::vector<Sphere> LoadSpheres(tinyxml2::XMLElement *elem);