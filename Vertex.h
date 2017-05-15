//
// Created by Göksu Güvendiren on 06/03/2017.
//

#pragma once
#include <vector>
#include "tinyxml/tinyxml2.h"
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

class Vertex
{
    glm::vec3 data;
    glm::vec3 normal;
    glm::vec2 uv;

    int id;

public:
    Vertex(int i = 0, glm::vec3 d = {0, 0, 0}, glm::vec3 n = {1, 0, 0}, glm::vec2 _uv = {0, 0}) : data(d), normal(n), uv(_uv), id(i)
    {}

    auto Data() const { return data; }
    auto Normal() const { return normal; }
    auto Normal(glm::vec3 n) { normal = n; }
    auto ID() const { return id; }

    void SetUV(glm::vec2 _uv) { uv = _uv; }
    glm::vec2 GetUV() const { return uv; }
};

std::vector<Vertex> LoadVertexData(tinyxml2::XMLElement *elem, const std::vector<glm::vec2>& texcoords);
std::vector<glm::vec2> LoadTexCoordData(tinyxml2::XMLElement *elem);
