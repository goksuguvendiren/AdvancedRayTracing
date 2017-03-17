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
    glm::vec3 normal;

public:
    Vertex(glm::vec3 d = {0, 0, 0}) : data(d) {}
    auto Data() const { return data; }
    auto Normal() const { return normal; }
    auto Normal(glm::vec3 n) { normal = n; }
};

std::vector<glm::vec3> LoadVertexData(tinyxml2::XMLElement *elem);