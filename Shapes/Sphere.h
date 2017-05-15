//
// Created by Göksu Güvendiren on 24/02/2017.
//

#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include "../Material.h"
//#include "../Materials/ClassicMaterial.hpp"
#include "Shape.h"
#include "../Vertex.h"
#include "../Texture.hpp"

class Ray;
class HitInfo;
class Vertex;
class ClassicMaterial;

class Sphere : public Shape
{
    int id;
    float radius;
    Vertex center;
    const Material* material;
    const ClassicMaterial* classic_material;
    const Texture* texture;

    glm::vec3 maxval;
    glm::vec3 minval;

    glm::mat4 transformationMatrix;
    glm::mat4 inverseTrMatrix;
    glm::mat4 inverseTranspose;

public:
    Sphere(int sid, float rd, Vertex c, int mid, int tid);
    boost::optional<HitInfo> Hit(const Ray &inverseRay) const;
    boost::optional<float>   ShadowHit(const Ray& ray) const;
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

    glm::vec2 GetTexCoords(glm::vec3 pos) const;
    std::pair<glm::vec3, glm::vec3> GradientVectors(const glm::vec2& uv, const glm::vec3& localCoord) const;
};

std::vector<Sphere> LoadSpheres(tinyxml2::XMLElement *elem);
