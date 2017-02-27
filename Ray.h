//
// Created by Göksu Güvendiren on 24/02/2017.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "glm/vec3.hpp"

class Ray
{
    glm::vec3 origin;
    glm::vec3 direction;

public:
    Ray() : origin({0, 0, 0}), direction({1, 0, 0}) {};
    Ray(glm::vec3 o, glm::vec3 d) : origin(o), direction(d) {};

    glm::vec3 Origin() const { return origin; }
    glm::vec3 Direction() const { return direction; }
};


#endif //RAYTRACER_RAY_H
