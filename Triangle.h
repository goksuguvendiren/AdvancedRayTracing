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

class Triangle : public Shape
{
    glm::vec3 pointA;
    glm::vec3 pointB;
    glm::vec3 pointC;

    glm::vec3 surfNormal;
    const Material* material;
    int id;

public:
    Triangle(glm::vec3 a = {0, 0, 0}, glm::vec3 b = {0, 0, 0}, glm::vec3 c = {0, 0, 0},
             int mid = 1, int tid = 1);

    ~Triangle();

    const Material* Material() const;
    std::pair<bool, HitInfo> Hit (const Ray& ray) const;
    bool FastHit(const Ray& ray) const;
    int ID() const;
};

std::vector<Triangle> CreateTriangles(tinyxml2::XMLElement* elem);

