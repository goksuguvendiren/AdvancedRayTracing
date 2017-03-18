//
// Created by Göksu Güvendiren on 24/02/2017.
//

#pragma once

#include <vector>
#include "glm/vec3.hpp"
#include "Material.h"
#include "Shape.h"
#include "Vertex.h"

class Ray;
class HitInfo;
class Vertex;

class Sphere : public Shape
{
    int id;
    float radius;
    Vertex center;
    int materialID;

public:
    Sphere(int sid = 1, float rd = 1.0f, Vertex c = Vertex{1, glm::vec3{1.0f, 0.0f, 0.0f}}, int mid = 1);
    boost::optional<HitInfo> Hit(const Ray &ray) const;
    bool FastHit (const Ray &ray) const;
    int ID() const { return id; }
};

std::vector<Sphere> LoadSpheres(tinyxml2::XMLElement *elem);