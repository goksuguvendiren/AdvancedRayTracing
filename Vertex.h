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

    int id;

public:
    Vertex(int i = 0, glm::vec3 d = {0, 0, 0}, glm::vec3 n = {1, 0, 0}) : id(i), data(d), normal(n) {}

    auto Data() const { return data; }
    auto Normal() const { return normal; }
    auto Normal(glm::vec3 n) { normal = n; }
    auto ID() const { return id; }
};

std::vector<Vertex> LoadVertexData(tinyxml2::XMLElement *elem);