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

class Materialx
{
public:
    virtual int ID() const = 0;
    virtual glm::vec3 ComputeReflectance(const HitInfo& hit, const Light& light) const = 0;
};

//std::vector<Material> LoadMaterials(tinyxml2::XMLElement *elem);
