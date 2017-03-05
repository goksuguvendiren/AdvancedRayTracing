//
// Created by Göksu Güvendiren on 24/02/2017.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "glm/vec3.hpp"
#include "Material.h"

class Ray;
class HitInfo;

class Sphere
{
    float radius;
    glm::vec3 center;
    unsigned int materialID;

public:
    Sphere() : radius(1.0f), center({0, 0, 0}), materialID(1) {}
    Sphere(float rd, glm::vec3 c, unsigned int id) : radius(rd), center(c), materialID(id) {}
    std::pair<bool, HitInfo> Hit(const Ray &ray);
};


#endif //RAYTRACER_SPHERE_H