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
    float roughness;

    bool isMirror;
    bool isTransparent;

    int id;

public:
    Material(int i, glm::vec3 a, glm::vec3 d, glm::vec3 s,
             glm::vec3 m, glm::vec3 t, float r = 1.0f, float p = 1, float rough = 0.f,
             bool ism = false, bool ist = false) : id(i), isMirror(ism), isTransparent(ist),
                                                   diffuse(d), specular(s), ambient(a),
                                                   phongExponent(p), roughness(rough),
                                                   mirrorReflectance(m), transparency(t),
                                                   refractionIndex(r){}

    glm::vec3 Diffuse() const { return diffuse; }
    glm::vec3 Specular() const { return specular; }
    glm::vec3 Ambient() const { return ambient; }
    glm::vec3 Mirror() const { return mirrorReflectance; }
    glm::vec3 Transparency() const { return transparency; }

    bool IsMirror() const { return isMirror; }
    bool IsTransparent() const { return isTransparent; }

    float PhongExp() const { return phongExponent; }
    float Roughness() const { return roughness; }
    float RefractionIndex() const { return refractionIndex; }

    auto ID() const { return id; }
};

std::vector<Material> LoadMaterials(tinyxml2::XMLElement *elem);