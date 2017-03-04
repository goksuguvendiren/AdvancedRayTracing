//
// Created by Göksu Güvendiren on 27/02/2017.
//

#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include "glm/glm.hpp"
#include "Ray.h"
#include "HitInfo.h"

class Triangle
{
    glm::vec3 pointA;
    glm::vec3 pointB;
    glm::vec3 pointC;

    glm::vec3 surfNormal;

public:
    Triangle(glm::vec3 a = {0, 0, 0}, glm::vec3 b = {1, 0, 0}, glm::vec3 c = {0, 1, 0}) : pointA(a),
                                                                                          pointB(b),
                                                                                          pointC(c)
    {
        surfNormal = glm::normalize(glm::cross(pointC - pointA, pointB - pointA));
    }

    std::pair<bool, HitInfo> Hit(const Ray& ray); //TODO : replace the return type with HitInfo
};


#endif //RAYTRACER_TRIANGLE_H
