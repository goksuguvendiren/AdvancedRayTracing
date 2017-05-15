//
// Created by Göksu Güvendiren on 06/03/2017.
//

#include <vector>
#include <sstream>
#include "Vertex.h"
#include <glm/vec2.hpp>

Vertex GetVertex(std::istringstream& stream, int id)
{
    Vertex vert;

    float datax;
    float datay;
    float dataz;

    stream >> datax;
    stream >> datay;
    stream >> dataz;

    return Vertex{id, glm::vec3{datax, datay, dataz}};
}

std::vector<Vertex> LoadVertexData(tinyxml2::XMLElement *elem, const std::vector<glm::vec2>& texcoords)
{
    std::istringstream stream { elem->GetText() };

    std::vector<Vertex> verts;
    int id = 0;
    while(stream){
        verts.push_back(GetVertex(stream, id++));
        assert(id == verts.size());
    }

    return verts;
}

std::vector<glm::vec2> LoadTexCoordData(tinyxml2::XMLElement *elem)
{
    auto Get_UV = [](std::istringstream& stream)
    {
        float data_u;
        float data_v;

        stream >> data_u;
        stream >> data_v;

        return glm::vec2{data_u, data_v};
    };

    std::istringstream stream { elem->GetText() };

    std::vector<glm::vec2> coords;
    while(stream)
    {
        coords.push_back(Get_UV(stream));
    }

    return coords;
}
