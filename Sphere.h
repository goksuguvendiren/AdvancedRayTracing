//
// Created by Göksu Güvendiren on 24/02/2017.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "glm/vec3.hpp"

class Ray;
class Sphere
{
    float radius;
    glm::vec3 center;

public:
    Sphere() : radius(1.0f), center({0, 0, 0}) {}
    Sphere(float rd, glm::vec3 c) : radius(rd), center(c) {}
    bool Hit(const Ray& ray); //TODO : replace the return type with HitInfo
};


#endif //RAYTRACER_SPHERE_H
