//
// Created by Göksu Güvendiren on 05/13/2017.
//

#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include "../tinyxml/tinyxml2.h"

class HitInfo;
class Light;
class BRDF;

class LightMaterial : public Materialx
{
    int id;
    glm::vec3 radiance;
    
public:
    LightMaterial(int i, glm::vec3 rad) : id(i), radiance(rad) {}
    int ID() const { return id; }
    glm::vec3 ComputeReflectance(const HitInfo& hit, const Light& light) const { return radiance; }
};

//std::vector<Material> LoadMaterials(tinyxml2::XMLElement *elem);
