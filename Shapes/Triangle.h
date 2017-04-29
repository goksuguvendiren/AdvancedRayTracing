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
    const Texture* texture;

    int id;
    bool smooth;

    Box bbox;

public:
    Triangle() = default;
    Triangle(Vertex a, Vertex b, Vertex c, int mid, int tid = -1, int tr_id = 1, bool sm = false);

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

    glm::vec2 GetTexCoords(glm::vec3 pos, float beta, float gamma) const;
    std::pair<glm::vec3, glm::vec3> GradientVectors(glm::vec3 normal) const;
};

std::vector<Triangle> LoadTriangles(tinyxml2::XMLElement* elem);

