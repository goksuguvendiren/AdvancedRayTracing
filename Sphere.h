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
    Material mat;

public:
    Sphere() : radius(1.0f), center({0, 0, 0}) {}
    Sphere(float rd, glm::vec3 c) : radius(rd), center(c) {}
    std::pair<bool, HitInfo> Hit(const Ray &ray);
};


#endif //RAYTRACER_SPHERE_H
