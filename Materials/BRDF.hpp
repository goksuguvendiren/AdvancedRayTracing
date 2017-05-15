//
// Created by Göksu Güvendiren on 05/13/2017.
//

#pragma once

#include <vector>
#include <string>
#include <glm/vec3.hpp>
#include "../tinyxml/tinyxml2.h"

class HitInfo;
class Light;
class ClassicMaterial;

class BRDF
{
public:
    virtual int ID() const = 0;
    virtual glm::vec3 ComputeReflectance(const HitInfo& hit, const Light& light, const ClassicMaterial& material) const = 0;
};

std::vector<std::unique_ptr<BRDF>> LoadBRDFs(tinyxml2::XMLElement *elem);
