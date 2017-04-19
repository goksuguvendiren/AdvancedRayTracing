//
// Created by Göksu Güvendiren on 27/02/2017.
//

#pragma once

#include "../glm/glm.hpp"
#include "../Ray.h"
#include "../HitInfo.h"
#include "../tinyxml/tinyxml2.h"
#include "../Scene.h"

class Material;
class Vertex;
class Box;

class Triangle : public Shape
{
    Vertex pointA;
    Vertex pointB;
    Vertex pointC;

    glm::vec3 surfNormal;
    const Material* material;

    int id;
    bool smooth;

    Box bbox;

public:
    Triangle() = default;
    Triangle(Vertex a, Vertex b, Vertex c, const Material* mat, int tid = 1, bool sm = false);

    ~Triangle();

    const Material* Mat() const;
    boost::optional<HitInfo> Hit (const Ray& ray) const;
    bool FastHit(const Ray& ray) const;
    int ID() const;

    auto Normal() const { return surfNormal; }

    Vertex& PointA() { return pointA; }
    Vertex& PointB() { return pointB; }
    Vertex& PointC() { return pointC; }

    Vertex PointA() const { return pointA; }
    Vertex PointB() const { return pointB; }
    Vertex PointC() const { return pointC; }

    glm::vec3 Min() const;
    glm::vec3 Max() const;

    glm::vec3 Middle() const;
};

std::vector<Triangle> LoadTriangles(tinyxml2::XMLElement* elem);
