//
// Created by Göksu Güvendiren on 04/03/2017.
//

#pragma once

#include <vector>
#include "glm/vec3.hpp"
#include "tinyxml/tinyxml2.h"

class Material
{
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 ambient;
    float phongExponent;

    int id;

public:
    Material(int i = 0, glm::vec3 a = {0, 0, 0}, glm::vec3 d = {0, 0, 0}, glm::vec3 s = {0, 0, 0}, float p = 1) : id(i),
                                                                                                                  diffuse(d),
                                                                                                                  specular(s),
                                                                                                                  ambient(a),
                                                                                                                  phongExponent(p){}
    glm::vec3 Diffuse() const { return diffuse; }
    glm::vec3 Specular() const { return specular; }
    glm::vec3 Ambient() const { return ambient; }

    float PhongExp() const { return phongExponent; }
};

std::vector<Material> LoadMaterials(tinyxml2::XMLElement *elem);