//
// Created by Göksu Güvendiren on 27/02/2017.
//

#pragma once

#include "glm/glm.hpp"
#include "Ray.h"
#include "HitInfo.h"
#include "tinyxml/tinyxml2.h"
#include "Scene.h"

class Triangle
{
    int pointA;
    int pointB;
    int pointC;

    glm::vec3 surfNormal;
    int materialID;
    int ID;

public:
    Triangle(int a = 1, int b = 1, int c = 1,
             int mid = 1, int tid = 1);

    ~Triangle();

    int Material() { return materialID; }
    std::pair<bool, HitInfo> Hit(const Ray& ray); //TODO : replace the return type with HitInfo
};

std::vector<Triangle> CreateTriangles(tinyxml2::XMLElement* elem);

