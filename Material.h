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

    glm::vec3 mirrorReflectance;
    glm::vec3 transparency;

    float refractionIndex;
    float phongExponent;

    bool isMirror;
    bool isTransparent;

    int id;

public:
    Material(int i = 0, glm::vec3 a = {0, 0, 0}, glm::vec3 d = {0, 0, 0}, glm::vec3 s = {0, 0, 0},
             glm::vec3 m = {0, 0, 0}, glm::vec3 t = {0, 0, 0}, float r = 1.0f, float p = 1,
             bool ism = false, bool ist = false) : id(i), isMirror(ism), isTransparent(ist),
                                                   diffuse(d), specular(s), ambient(a),
                                                   phongExponent(p), mirrorReflectance(m),
                                                   transparency(t), refractionIndex(r){}

    glm::vec3 Diffuse() const { return diffuse; }
    glm::vec3 Specular() const { return specular; }
    glm::vec3 Ambient() const { return ambient; }
    glm::vec3 Mirror() const { return mirrorReflectance; }
    glm::vec3 Transparency() const { return transparency; }

    bool IsMirror() const { return isMirror; }
    bool IsTransparent() const { return isTransparent; }

    float PhongExp() const { return phongExponent; }
    float RefractionIndex() const { return refractionIndex; }
};

std::vector<Material> LoadMaterials(tinyxml2::XMLElement *elem);