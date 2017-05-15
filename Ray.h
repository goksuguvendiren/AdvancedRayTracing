//
// Created by Göksu Güvendiren on 24/02/2017.
//

#pragma once

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

class Ray
{
    glm::vec3 origin;
    glm::vec3 direction;
    glm::vec3 invdirection;

public:
    Ray() : origin({0, 0, 0}), direction({1, 0, 0}), invdirection(glm::vec3(1.0f, 1.f, 1.f) / direction) {};
    Ray(glm::vec3 o, glm::vec3 d) : origin(o), direction(d)
    {
//        direction = glm::normalize(d);
//        assert(glm::length(direction) < 1.001f && glm::length(direction) > 0.99f);
        invdirection = glm::vec3(1.0f, 1.f, 1.f) / direction;
    };

    glm::vec3 Origin() const { return origin; }
    glm::vec3 Direction() const { return direction; }
    glm::vec3 InvDirection() const { return invdirection; }
};
