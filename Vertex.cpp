//
// Created by Göksu Güvendiren on 06/03/2017.
//

#include <vector>
#include <sstream>
#include "Vertex.h"

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

std::vector<Vertex> LoadVertexData(tinyxml2::XMLElement *elem)
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