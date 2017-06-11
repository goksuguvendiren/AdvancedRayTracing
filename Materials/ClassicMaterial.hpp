//
// Created by Göksu Güvendiren on 05/13/2017.
//


#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include "../Scene.h"
#include "../tinyxml/tinyxml2.h"
#include "Materialx.hpp"

class BRDF;
class HitInfo;
extern Scene scene;

class ClassicMaterial : public Materialx
{
    int id;
    
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 ambient;
    
    float phong_exp;
    
    const BRDF* brdf_material;
    
public:
    ClassicMaterial(int i, glm::vec3 a, glm::vec3 d, glm::vec3 s,
                    float p = 1, int bid = -1) : id(i), diffuse(d), specular(s), ambient(a),
                                                 phong_exp(p), brdf_material(nullptr)
    {
        if (bid != -1)
        {
            brdf_material = scene.GetBRDF(bid);
        }
    }
        
    glm::vec3 Diffuse() const { return diffuse; }
    glm::vec3 Specular() const { return specular; }
    glm::vec3 Ambient() const { return ambient; }
    
    float PhongExp() const { return phong_exp; }
        
    int ID() const { return id; }
    glm::vec3 ComputeReflectance(const HitInfo& hit, const Light& light) const;
    bool is_BRDF() const { return brdf_material != nullptr; }
    
    glm::vec3 CalculateColor(const HitInfo& hit, int recdepth) const;
};

//std::vector<Material> LoadMaterials(tinyxml2::XMLElement *elem);
