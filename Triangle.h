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
    Triangle() = default;
    Triangle(Vertex a, Vertex b, Vertex c, int mid = 1, int tid = 1);

    ~Triangle();

    const Material* Material() const;
    boost::optional<HitInfo> Hit (const Ray& ray) const;
    bool FastHit(const Ray& ray) const;
    int ID() const;

    auto Normal() const { return surfNormal; }
    Vertex& PointA() { return pointA; }
    Vertex& PointB() { return pointB; }
    Vertex& PointC() { return pointC; }
};

std::vector<Triangle> LoadTriangles(tinyxml2::XMLElement* elem);

