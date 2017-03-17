//
// Created by Göksu Güvendiren on 27/02/2017.
//

#pragma once

#include "glm/glm.hpp"
#include "Ray.h"
#include "HitInfo.h"
#include "tinyxml/tinyxml2.h"
#include "Scene.h"

class Material;
class Vertex;

class Triangle : public Shape
{
    Vertex pointA;
    Vertex pointB;
    Vertex pointC;

    glm::vec3 surfNormal;
    const Material* material;
    int id;

public:
    Triangle(Vertex a = {0, 0, 0}, Vertex b = {0, 0, 0}, Vertex c = {0, 0, 0},
             int mid = 1, int tid = 1);

    ~Triangle();

    const Material* Material() const;
    std::pair<bool, HitInfo> Hit (const Ray& ray) const;
    bool FastHit(const Ray& ray) const;
    int ID() const;
};

std::vector<Triangle> LoadTriangles(tinyxml2::XMLElement* elem);

