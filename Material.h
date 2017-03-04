//
// Created by Göksu Güvendiren on 04/03/2017.
//

#pragma once

#include "glm/vec3.hpp"

class Material
{
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 ambient;

public:
    Material(glm::vec3 d = {0, 0, 0}, glm::vec3 s = {0, 0, 0}, glm::vec3 a = {0, 0, 0}) : diffuse(d),
                                                                                          specular(s),
                                                                                          ambient(a) {}
    glm::vec3 Diffuse() { return diffuse; }
    glm::vec3 Specular() { return specular; }
    glm::vec3 Ambient() { return ambient; }
};
