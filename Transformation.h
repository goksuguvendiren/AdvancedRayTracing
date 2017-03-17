//
// Created by Göksu Güvendiren on 15/03/2017.
//

#pragma once

#include <map>
#include "glm/detail/type_mat.hpp"
#include "tinyxml/tinyxml2.h"

enum class Transformations
{
    Translation,
    Scaling,
    Rotation
};

std::map<std::string, glm::mat4> CreateTransformations(tinyxml2::XMLElement* elem);
std::map<std::string, glm::mat4> CreateTranslations(tinyxml2::XMLElement* elem);
std::map<std::string, glm::mat4> CreateRotations(tinyxml2::XMLElement* elem);
std::map<std::string, glm::mat4> CreateScalings(tinyxml2::XMLElement* elem);
