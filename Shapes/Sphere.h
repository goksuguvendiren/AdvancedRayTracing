//
// Created by Göksu Güvendiren on 24/02/2017.
//

#pragma once

#include <vector>
#include "../glm/vec3.hpp"
#include "../Material.h"
#include "Shape.h"
#include "../Vertex.h"

class Ray;
class HitInfo;
class Vertex;

class Sphere : public Shape
{
    int id;
    float radius;
    Vertex center;
    const Material* material;

    glm::vec3 maxval;
    glm::vec3 minval;

    glm::mat4 transformationMatrix;
    glm::mat4 inverseTrMatrix;
    glm::mat4 inverseTranspose;

public:
    Sphere(int sid = 1, float rd = 1.0f, Vertex c = Vertex{1, glm::vec3{1.0f, 0.0f, 0.0f}}, int mid = 1);
    boost::optional<HitInfo> Hit(const Ray &inverseRay) const;
    bool FastHit (const Ray &ray) const;
    int ID() const { return id; }

    void TransformationMatrix(const glm::mat4& mat)
    {
        transformationMatrix = mat;
        inverseTrMatrix = glm::inverse(transformationMatrix);
        inverseTranspose = glm::transpose(inverseTrMatrix);
    }

    glm::vec3 Max() const { return maxval; }
    glm::vec3 Min() const { return minval; }

    glm::vec3 Middle() const { return center.Data(); }
};

std::vector<Sphere> LoadSpheres(tinyxml2::XMLElement *elem);