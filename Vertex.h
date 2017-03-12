//
// Created by Göksu Güvendiren on 06/03/2017.
//

#pragma once
#include <vector>
#include "glm/vec3.hpp"
#include "tinyxml/tinyxml2.h"

class Vertex
{
    glm::vec3 data;
public:
    Vertex(glm::vec3 d = {0, 0, 0}) : data(d) {}
    auto Data() const { return data; }
};

std::vector<glm::vec3> CreateVertexData(tinyxml2::XMLElement* elem);